#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Platform.h"
// #include "Game.h"

// void Platform::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
// {
// 	// spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
// 	spritesheet.loadFromFile("images/spider_sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
// 	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
//
// 	tileMapDispl = tileMapPos;
// 	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlatform.x), float(tileMapDispl.y + posPlatform.y)));
// }
//
// void Platform::update(int deltaTime)
// {
// 	int playerMovementSpeed = 2;
// 	sprite->update(deltaTime);
//
// 	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlatform.x), float(tileMapDispl.y + posPlatform.y)));
// }
//
// void Platform::render()
// {
// 	sprite->render();
// }

// void Platform::setTileMap(TileMap *tileMap)
// {
// 	map = tileMap;
// }
//
// void Platform::setPosition(const glm::vec2 &pos)
// {
// 	posPlatform = pos;
// 	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlatform.x), float(tileMapDispl.y + posPlatform.y)));
// }
