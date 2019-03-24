#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE


#include "Sprite.h"
// #include "TileMap.h"


// Platform is a Sprite that represents a moving platform in game. As such it has
// all properties it needs to track its path, movement, carrying the player and collisions.


class Platform
{

public:
	// void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	// void update(int deltaTime);
	// void render();
	//
	// void setTileMap(TileMap *tileMap);
	// void setPosition(const glm::vec2 &pos);

	void setTileID(int tileID){ this->tileID = tileID; }
	void setID(int ID){ this->ID = ID; }
	int getID(){ return this->ID; }
	void setSpawn(glm::vec2 initPos){ this->initPos = initPos; }
	void setSize(glm::vec2 size){ this->size = size; }
	void setPathStart(glm::vec2 pathStart){ this->pathStart = pathStart; }
	void setPathEnd(glm::vec2 pathEnd){ this->pathEnd = pathEnd; }

private:
	glm::ivec2 tileMapDispl, posPlatform;
	Texture spritesheet;
	Sprite *sprite;
	// TileMap *map;

	int ID; // like in object layer of level
	int tileID; // index in stylesheet
	glm::vec2 size, initPos, pathStart, pathEnd;
	// TODO subtract size from pathEnd to get final topLeftEdge position

};


#endif // _PLATFORM_INCLUDE
