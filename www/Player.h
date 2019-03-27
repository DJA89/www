#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "SavedState.h"
#include "SoundSystem.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void playerFalling(int);
	bool hasDied();
	void restorePlayerPosition(bool upsidedown, glm::ivec2 normalizedCheckpointPosition);

	void handleCollisionWithPlatform(Platform & platform);

	glm::ivec2 getPosition(){ return posPlayer; }
	void setPositionX(int xPosition){ this->posPlayer.x = xPosition; }
	void setPositionY(int yPosition){ this->posPlayer.y = yPosition; }
	glm::ivec2 getSize(){ return sizePlayer; }
	bool getIfUpSideDown(){ return upsidedown; }
	void endGame();



private:
	glm::ivec2 posPlayer; // TODO change to glm::vec2
	glm::ivec2 sizePlayer; // TODO change to glm::vec2
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int playerMovementSpeed = 4;
	bool upsidedown, actionPressedBeforeCollition, dying;
	int framesSinceDeath;
	bool renderInDeath();
	bool isStandingOnPlatform;
	Platform * standingOn;
	SoundSystemClass sound;
	SoundClass soundSample;
};


#endif // _PLAYER_INCLUDE
