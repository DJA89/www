#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPACEBAR 32

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STAND_LEFTU, STAND_RIGHTU, MOVE_LEFTU, MOVE_RIGHTU
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	upsidedown = false;
	actionPressedBeforeCollition = false;
	// spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.loadFromFile("images/spider_sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizePlayer = glm::vec2(32, 32);
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

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

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	int playerMovementSpeed = 2;
	sprite->update(deltaTime);
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::playerFalling(int pixels) {
	posPlayer.y += pixels;
	bool collition = map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y) ||
		map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y);
	if(collition) {
		if (!actionPressedBeforeCollition) {
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(SPACEBAR)) {
				actionPressedBeforeCollition = true;
				upsidedown = !upsidedown;
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
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}
