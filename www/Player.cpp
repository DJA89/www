#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	// spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.loadFromFile("images/spider_sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
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
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= playerMovementSpeed;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += playerMovementSpeed;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += playerMovementSpeed;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= playerMovementSpeed;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	cout << posPlayer.x << ",\t" << posPlayer.y << endl;

	// actualize player velocity & player position
	vPlayer.y -= gravity;
	posPlayer.y += vPlayer.y;

	// handle jumps
	if (bJumping){
		if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)){
			// if hit ceiling => stop moving upwards
			vPlayer.y = 0;
		}
		// if hit ground stop jump
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)){
			bJumping = false;
			vPlayer.y = 0;
		}
	} else {
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)){
			vPlayer.y = 0;
		}
		// jump key hit
		if(Game::instance().getSpecialKey(GLUT_KEY_UP)){
			// jump
			bJumping = true;
			vPlayer.y = -8;
		}
	}

	// if(bJumping)
	// {
	// 	jumpAngle += JUMP_ANGLE_STEP;
	// 	if(jumpAngle == 180)
	// 	{
	// 		bJumping = false;
	// 		posPlayer.y = startY;
	// 	}
	// 	else
	// 	{
	// 		posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
	// 		if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)){
	// 			bJumping = false;
	// 		}
	// 		if(jumpAngle > 90){
	// 			bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
	// 		}
	// 	}
	// }
	// else
	// {
	// 	posPlayer.y += FALL_STEP;
	// 	if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
	// 	{
	// 		if(Game::instance().getSpecialKey(GLUT_KEY_UP))
	// 		{
	// 			// start jump
	// 			bJumping = true;
	// 			jumpAngle = 0;
	// 			startY = posPlayer.y;
	// 		}
	// 	}
	// }

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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




