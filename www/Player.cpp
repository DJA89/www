#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "BoundingShape.h"

#define FALL_STEP 6
#define SPACEBAR 32

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STAND_LEFTU, STAND_RIGHTU, MOVE_LEFTU, MOVE_RIGHTU,
	DEATH_LEFT, DEATH_RIGHT, DEATH_LEFTU, DEATH_RIGHTU
};


void Player::init(ShaderProgram &shaderProgram)
{
	upsidedown = false;
	actionPressedBeforeCollition = false;
	dying = false;
	framesSinceDeath = 0;

	// spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.loadFromFile("images/spider_sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizePlayer = glm::vec2(32, 32);
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
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
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));


	SoundSystemClass sound = SoundSystemClass();

	sound.createSound(&soundSample, "sounds/jump.mp3");
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
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
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
		posPlayer.x -= playerMovementSpeed;
		if(map->collisionMoveLeft(posPlayer, sizePlayer))
		{
			posPlayer.x += playerMovementSpeed;
			if (upsidedown) {
				sprite->changeAnimation(STAND_LEFTU);
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
			}
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
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
		posPlayer.x += playerMovementSpeed;
		if(map->collisionMoveRight(posPlayer, sizePlayer))
		{
			posPlayer.x -= playerMovementSpeed;
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
	// if moving on a platform, add platform velocity
	if (isStandingOnPlatform && standingOn != NULL){
		posPlayer += standingOn->getVelocity();
		isStandingOnPlatform = false; // wait for next collision
	}

	// check for collisions
	if (map->triggerDeath(posPlayer, sizePlayer, upsidedown)) {
		// detach from platform
		isStandingOnPlatform = false;
		standingOn = NULL;
		// die
		dying = true;
		int currentAnimation = sprite->animation();
		int newAnimation;
		if (currentAnimation == MOVE_LEFT || currentAnimation == STAND_LEFT) {
			newAnimation = DEATH_LEFT;
		} else if (currentAnimation == MOVE_RIGHT || currentAnimation == STAND_RIGHT) {
			newAnimation = DEATH_RIGHT;
		} else if (currentAnimation == MOVE_RIGHTU || currentAnimation == STAND_RIGHTU) {
			newAnimation = DEATH_RIGHTU;
		} else if (currentAnimation == MOVE_LEFTU || currentAnimation == STAND_LEFTU) {
			newAnimation = DEATH_LEFTU;
		}
		sprite->changeAnimation(newAnimation);
	}

	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

// center means in the middle of the player touching the floor/ceiling it is standing on
void Player::restorePlayerPosition(bool upsidedown, glm::ivec2 normalizedCheckpointPosition){
	// player specific restoring
	int xCenter = normalizedCheckpointPosition.x;
	int yCenter = normalizedCheckpointPosition.y;
	int xPos, yPos;
	if (upsidedown) {
		xPos = xCenter - sizePlayer.x/2;
		yPos = yCenter;
		sprite->changeAnimation(STAND_RIGHTU);
	} else {
		xPos = xCenter - sizePlayer.x/2;
		yPos = yCenter - sizePlayer.y;
		sprite->changeAnimation(STAND_RIGHT);
	}
	framesSinceDeath = 0;
	dying = false;
	this->upsidedown = upsidedown;
	setPosition(glm::ivec2(xPos, yPos));
}

void Player::playerFalling(int pixels) {
	posPlayer.y += pixels;
	bool collition = map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y) ||
		map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y);
	if(collition || isStandingOnPlatform) {
		if (!actionPressedBeforeCollition) {
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(SPACEBAR)) {
				actionPressedBeforeCollition = true;
				upsidedown = !upsidedown;
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
			if (!(Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(SPACEBAR))) {
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

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

void Player::handleCollisionWithPlatform(FixedPathEntity & platform) {
	glm::vec2 posPlayer_f = (glm::vec2)posPlayer;
	glm::vec2 sizePlayer_f = (glm::vec2)sizePlayer;
	BoundingShape * platBound = platform.getBoundingShape();
	float leftPlayerFoot = posPlayer_f.x;
	float rightPlayerFoot = posPlayer_f.x + sizePlayer_f.x;
	float leftPlatformBorder = platBound->getPosition().x;
	float rightPlatformBorder = platBound->getPosition().x + platBound->getSize().x;
	if (min(rightPlayerFoot, rightPlatformBorder) - max(leftPlayerFoot, leftPlatformBorder) >= minimalStandingFraction * sizePlayer_f.x){
		// can stand on platform
		if (posPlayer_f.y < platBound->getPosition().y){
			// is above
			posPlayer_f.y = platBound->getPosition().y - sizePlayer_f.y;
		} else if (posPlayer_f.y > platBound->getPosition().y){
			// is below
			posPlayer_f.y = platBound->getPosition().y + platBound->getSize().y;
		} else { // let's just hope we never get here
			cout << "ERROR: player was neither below, nor above platform" << endl;
		}
		isStandingOnPlatform = true; // stand on platform
		standingOn = &platform; // store platform to follow movement
	} else {
		// is next to platform, so just pull out
		if (posPlayer_f.x < platBound->getPosition().x){
			// is left
			posPlayer_f.x = platBound->getPosition().x - sizePlayer_f.x;
		} else if (posPlayer_f.x > platBound->getPosition().x){
			// is right
			posPlayer_f.x = platBound->getPosition().x + platBound->getSize().x;
		} else { // let's just hope we never get here
			cout << "ERROR: player was neither left, nor right of platform" << endl;
		}
	}
	posPlayer = (glm::ivec2) posPlayer_f;
	sizePlayer = (glm::ivec2) sizePlayer_f;
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

void Player::endGame() {
	sound.releaseSound(soundSample);
}

void Player::handleCollisionWithDeath(FixedPathEntity & enemy) {
	dying = true;
	int currentAnimation = sprite->animation();
	int newAnimation;
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
	sprite->changeAnimation(newAnimation);
}

bool Player::isDying() {
	return dying;
}
