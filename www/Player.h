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
	void loadState(SavedState &savedState);
	void playerFalling(int);
	bool hasDied();
	void restorePlayerPosition(bool upsidedown, glm::ivec2 normalizedCheckpointPosition);

	glm::ivec2 tileMapDispl, posPlayer, sizePlayer;
	bool upsidedown, actionPressedBeforeCollition, dying;

private:
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int framesSinceDeath;
	bool renderInDeath();

};


#endif // _PLAYER_INCLUDE
