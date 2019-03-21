#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "SavedState.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void loadState();
	void playerFalling(int);
	void initializeSavedState();

	glm::ivec2 tileMapDispl, posPlayer, sizePlayer;

private:
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool upsidedown, actionPressedBeforeCollition, dying;
	int framesSinceDeath;
	SavedState savedState;
	bool renderInDeath();

};


#endif // _PLAYER_INCLUDE
