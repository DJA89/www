#include "Player.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <algorithm>
#include "Game.h"
#include "BoundingShape.h"
#include "Sprite.h"
#include "TileMap.h"
#include "SavedState.h"
#include "SoundSystem.h"
#include "AxisAlignedBoundingBox.h"

#define FALL_STEP 9
#define SPACEBAR 32
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STAND_LEFTU, STAND_RIGHTU, MOVE_LEFTU, MOVE_RIGHTU,
	DEATH_LEFT, DEATH_RIGHT, DEATH_LEFTU, DEATH_RIGHTU
};

Player::~Player() {
	if (spritesheet != NULL){
		delete spritesheet;
	}
	// remove bounding shapes (created here)
	// don't need to remove this->collisionBounds: it points to one of these
	if (collisionBoundsUpright != NULL){
		delete collisionBoundsUpright;
	}
	if (collisionBoundsUpsidedown != NULL){
		delete collisionBoundsUpsidedown;
	}
}

void Player::init(ShaderProgram &shaderProgram)
{
	// load spritesheet
	spritesheet = new Texture();
	spritesheet->loadFromFile("images/spider_sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	// call Entity::init (now that we have the spritesheet for it)
	Entity::init(*spritesheet, shaderProgram);
	// general config
	upsidedown = false;
	actionPressedBeforeCollition = false;
	dying = false;
	framesSinceDeath = 0;
	collidingWithWall = false;
	speed = 4.f;
	this->size = glm::vec2(32, 32);
	// initialize manual boundingShapes (for upright and upsidedown)
	collisionBoundsUpright = new AxisAlignedBoundingBox(glm::vec2(0.f, 7.f), glm::vec2(35.f, 25.f));
	collisionBoundsUpsidedown = new AxisAlignedBoundingBox(glm::vec2(0.f, 0.f), glm::vec2(35.f, 25.f));
	this->setBoundingShape(collisionBoundsUpright);
	// sprite & animations
	sprite = Sprite::createSprite(size, glm::vec2(0.25, 0.25), spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

		sprite->setAnimationSpeed(STAND_LEFTU, 8);
		sprite->addKeyframe(STAND_LEFTU, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(STAND_RIGHTU, 8);
		sprite->addKeyframe(STAND_RIGHTU, glm::vec2(0.75f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFTU, 8);
		sprite->addKeyframe(MOVE_LEFTU, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(MOVE_LEFTU, glm::vec2(0.5f, 0.25f));
		sprite->addKeyframe(MOVE_LEFTU, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(MOVE_RIGHTU, 8);
		sprite->addKeyframe(MOVE_RIGHTU, glm::vec2(0.75, 0.f));
		sprite->addKeyframe(MOVE_RIGHTU, glm::vec2(0.75, 0.25f));
		sprite->addKeyframe(MOVE_RIGHTU, glm::vec2(0.75, 0.5f));

		sprite->setAnimationSpeed(DEATH_LEFT, 8);
		sprite->addKeyframe(DEATH_LEFT, glm::vec2(0.f, 0.75f));

		sprite->setAnimationSpeed(DEATH_RIGHT, 8);
		sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.25f, 0.75f));

		sprite->setAnimationSpeed(DEATH_LEFTU, 8);
		sprite->addKeyframe(DEATH_LEFTU, glm::vec2(0.5f, 0.75f));

		sprite->setAnimationSpeed(DEATH_RIGHTU, 8);
		sprite->addKeyframe(DEATH_RIGHTU, glm::vec2(0.75f, 0.75f));

	sprite->changeAnimation(STAND_RIGHT);
	sprite->setPosition(glm::vec2(position.x, position.y));


	SoundSystemClass sound = SoundSystemClass();

	sound.createSound(&soundSample, "sounds/jump.mp3");
	sound.createSound(&soundSample2, "sounds/death.wav");
}

void Player::flipCollisionBounds(){
	if (this->collisionBounds == collisionBoundsUpright){
		this->setBoundingShape(collisionBoundsUpsidedown);
	} else { // upsidedown
		this->setBoundingShape(collisionBoundsUpright);
	}
}

bool Player::isDying() {
	return dying;
}

bool Player::hasDied(){
	return dying && (framesSinceDeath > 60);
}

void Player::update(int deltaTime)
{
	if (dying) {
		return;
	}
	sprite->update(deltaTime);
	// keyboard controls
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT) || Game::instance().getKey(A_KEY))
	{
		if (upsidedown) {
			if (sprite->animation() != MOVE_LEFTU) {
				sprite->changeAnimation(MOVE_LEFTU);
			}
		}
		else {
			if (sprite->animation() != MOVE_LEFT) {
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
		position.x -= speed;
		if (map->collisionMoveLeft(position, size)) {
			if (isStandingOnMovingEntity && standingOn != NULL && dynamic_cast<ConveyorBelt*>(standingOn)) {
				ConveyorBelt * cb = dynamic_cast<ConveyorBelt*>(standingOn);
					float vel = cb->getVelocity().x;
					if (vel > 0) {
						position.x += vel;
							if (map->collisionMoveLeft(position, size)) {
								float tileSize = map->getTileSize();
									int aux = ((position.x) / tileSize);
									int aux2 = tileSize * (aux + 1);
									position.x = aux2 - vel;
							}
							else {
								position.x -= vel;
							}
						collidingWithWall = true;
					}
					else {
						position.x += speed;
					}
			}
			else {
				position.x += speed;
			}
			if (upsidedown) {
				sprite->changeAnimation(STAND_LEFTU);
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
			}
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getKey(D_KEY))
	{
		if (upsidedown) {
			if (sprite->animation() != MOVE_RIGHTU) {
				sprite->changeAnimation(MOVE_RIGHTU);
			}
		}
		else {
			if (sprite->animation() != MOVE_RIGHT) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		position.x += speed;
		if (map->collisionMoveRight(position, size)) {
			if (isStandingOnMovingEntity && standingOn != NULL && dynamic_cast<ConveyorBelt*>(standingOn)) {
				ConveyorBelt * cb = dynamic_cast<ConveyorBelt*>(standingOn);
				float vel = cb->getVelocity().x;
				if (vel < 0) {
					position.x += vel;
					if (map->collisionMoveLeft(position, size)) {
						float tileSize = map->getTileSize();
						int aux = (((position.x + size.x - 1) / tileSize) - 2);
						int aux2 = tileSize * aux;
						position.x = aux2 - vel;
					}
					else {
						position.x -= vel;
					}
					collidingWithWall = true;
				}
				else {
					position.x -= speed;
				}
			}
			else {
				position.x -= speed;
			}
			if (upsidedown) {
				sprite->changeAnimation(STAND_LEFTU);
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
			}
			if (upsidedown) {
				sprite->changeAnimation(STAND_RIGHTU);
			}
			else {
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
		else if (sprite->animation() == MOVE_LEFTU)
			sprite->changeAnimation(STAND_LEFTU);
		else if (sprite->animation() == MOVE_RIGHTU)
			sprite->changeAnimation(STAND_RIGHTU);
	}

	if (upsidedown) {
		playerFalling(-FALL_STEP);
	}
	else {
		playerFalling(FALL_STEP);
	}
	// if moved by entity, add entity velocity to player's
	if (isStandingOnMovingEntity && standingOn != NULL){
		position += standingOn->getVelocity();
		isStandingOnMovingEntity = false; // wait for next collision
	}

	sprite->setPosition(glm::vec2(position.x, position.y));
}

// center means in the middle of the player touching the floor/ceiling it is standing on
void Player::restorePlayerPosition(bool upsidedown, glm::vec2 normalizedCheckpointPosition){
	// player specific restoring
	int xCenter = normalizedCheckpointPosition.x;
	int yCenter = normalizedCheckpointPosition.y;
	int xPos, yPos;
	if (upsidedown) {
		xPos = xCenter - size.x/2;
		yPos = yCenter;
		sprite->changeAnimation(STAND_RIGHTU);
	} else {
		xPos = xCenter - size.x/2;
		yPos = yCenter - size.y;
		sprite->changeAnimation(STAND_RIGHT);
	}
	framesSinceDeath = 0;
	dying = false;
	this->upsidedown = upsidedown;
	setPosition(glm::vec2(xPos, yPos));
}

void Player::playerFalling(int pixels) {
	position.y += pixels;

	bool collition = collidingWithWall || map->collisionMoveUp(position, size, &position.y) ||
		map->collisionMoveDown(position, size, &position.y);
	collidingWithWall = false;
	if(collition || isStandingOnMovingEntity) {
		if (!actionPressedBeforeCollition) {
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(SPACEBAR) ||
				Game::instance().getSpecialKey(GLUT_KEY_DOWN) || Game::instance().getKey(S_KEY) ||
				Game::instance().getKey(W_KEY)) {
				actionPressedBeforeCollition = true;
				upsidedown = !upsidedown;
				flipCollisionBounds();
				sound.playSound(soundSample, false);
				switch (sprite->animation()) {
				case MOVE_LEFT:
					sprite->changeAnimation(MOVE_LEFTU);
					break;
				case MOVE_RIGHT:
					sprite->changeAnimation(MOVE_RIGHTU);
					break;
				case MOVE_LEFTU:
					sprite->changeAnimation(MOVE_LEFT);
					break;
				case MOVE_RIGHTU:
					sprite->changeAnimation(MOVE_RIGHT);
					break;
				case STAND_LEFT:
					sprite->changeAnimation(STAND_LEFTU);
					break;
				case STAND_RIGHT:
					sprite->changeAnimation(STAND_RIGHTU);
					break;
				case STAND_LEFTU:
					sprite->changeAnimation(STAND_LEFT);
					break;
				case STAND_RIGHTU:
					sprite->changeAnimation(STAND_RIGHT);
					break;
				}
			}
		}
		else {
			if (!(Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(SPACEBAR) ||
				Game::instance().getSpecialKey(GLUT_KEY_DOWN) || Game::instance().getKey(S_KEY) ||
				Game::instance().getKey(W_KEY))) {
				actionPressedBeforeCollition = false;
			}
		}
	}
}

void Player::render()
{
	if (dying) {
		framesSinceDeath += 1;
		if (renderInDeath()) {
			sprite->render();
		}
	}
	else{
		sprite->render();
	}
}

bool Player::renderInDeath() {
	bool ret = true;
	if (framesSinceDeath <= 15 && (framesSinceDeath % 10 == 0 || framesSinceDeath % 9 == 1)) {
		ret = false;
	}
	else if (framesSinceDeath > 15 && framesSinceDeath <= 30 && (framesSinceDeath % 7 == 0 || framesSinceDeath % 7 == 1)) {
		ret = false;
	}
	else if (framesSinceDeath > 30 && framesSinceDeath <= 45 && (framesSinceDeath % 5 == 0 || framesSinceDeath % 5 == 1)) {
		ret = false;
	}
	else if (framesSinceDeath > 45 && (framesSinceDeath % 3 == 0 || framesSinceDeath % 3 == 1)) {
		ret = false;
	}

	return ret;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::handleCollisionWithMap(TileMap & map){
	// get MTV (see: https://www.toptal.com/game/video-game-physics-part-ii-collision-detection-for-solid-objects)
	glm::vec2 mtv = map.getMinimumTranslationVector(*this);
	// correct player position by this vector
	position -= mtv;
}

void Player::handleCollisionWithMovingEntity(Entity & platform) {
	glm::vec2 position_f = (glm::vec2)position;
	glm::vec2 size_f = (glm::vec2)size;
	BoundingShape * platBound = platform.getBoundingShape();
	float leftPlayerFoot = position_f.x;
	float rightPlayerFoot = position_f.x + size_f.x;
	float leftPlatformBorder = platBound->getPosition().x;
	float rightPlatformBorder = platBound->getPosition().x + platBound->getSize().x;
	float minStanding;
	if (dynamic_cast<ConveyorBelt*> (&platform)) {
		minStanding = minimalStandingFractionConveyorBelt;
	}
	else {
		minStanding = minimalStandingFractionPlatform;
	}
	if (min(rightPlayerFoot, rightPlatformBorder) - max(leftPlayerFoot, leftPlatformBorder) >= minStanding * size_f.x){
		// can stand on platform
		if (true){//!dynamic_cast<ConveyorBelt*> (&platform)) {
			if (position_f.y < platBound->getPosition().y) {
				// is above
				position_f.y = platBound->getPosition().y - size_f.y;
			}
			else if (position_f.y > platBound->getPosition().y) {
				// is below
				position_f.y = platBound->getPosition().y + platBound->getSize().y;
			}
			else { // honestly I don't know why we can get here...
		 // cout << "ERROR: player was neither below, nor above platform" << endl;
			}
		}
		isStandingOnMovingEntity = true; // stand on platform
		standingOn = &platform; // store platform to follow movement
	} else {
		// is next to platform, so just pull out
		if (position_f.x < platBound->getPosition().x){
			// is left
			position_f.x = platBound->getPosition().x - size_f.x;
		} else if (position_f.x > platBound->getPosition().x){
			// is right
			position_f.x = platBound->getPosition().x + platBound->getSize().x;
		} else { // honestly I don't know why we can get here...
			// cout << "ERROR: player was neither left, nor right of platform" << endl;
		}
	}
	position = position_f;
	size = size_f;
	sprite->setPosition(glm::vec2(position.x, position.y));
}

void Player::endGame() {
	sound.releaseSound(soundSample);
}

void Player::handleCollisionWithDeath(Entity & e) {
	// detach from platform
	isStandingOnMovingEntity = false;
	standingOn = NULL;
	// die
	dying = true;
	int currentAnimation = sprite->animation();
	int newAnimation = currentAnimation; // make sure it is initialized
	if (currentAnimation == MOVE_LEFT || currentAnimation == STAND_LEFT) {
		newAnimation = DEATH_LEFT;
	}
	else if (currentAnimation == MOVE_RIGHT || currentAnimation == STAND_RIGHT) {
		newAnimation = DEATH_RIGHT;
	}
	else if (currentAnimation == MOVE_RIGHTU || currentAnimation == STAND_RIGHTU) {
		newAnimation = DEATH_RIGHTU;
	}
	else if (currentAnimation == MOVE_LEFTU || currentAnimation == STAND_LEFTU) {
		newAnimation = DEATH_LEFTU;
	}
	sound.playSound(soundSample2, false);

	sprite->changeAnimation(newAnimation);

}

void Player::handleCollision(Entity & e) {
	e.handleCollision(*this);
}
