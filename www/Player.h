#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Entity.h"
#include "SoundSystem.h" // includes SoundSystemClass and SoundClass

class Sprite;
class TileMap;
class SavedState;


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Entity
{

public:
	~Player();
	void init(ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void playerFalling(int);
	bool hasDied();
	bool isDying();
	void restorePlayerPosition(bool upsidedown, glm::vec2 normalizedCheckpointPosition);

	void handleCollisionWithMovingEntity(Entity & platform);
	void handleCollisionWithDeath(Entity & e);
	void handleCollisionWithMap(TileMap & map);
	void handleCollision(Entity & e);

	void setPositionX(int xPosition){ this->position.x = xPosition; }
	void setPositionY(int yPosition){ this->position.y = yPosition; }
	bool getIfUpSideDown(){ return upsidedown; }
	void endGame();

private:
	bool renderInDeath();

private:
	Texture * spritesheet;
	TileMap *map; // TODO extract to Scene
	float minimalStandingFractionPlatform = 0.5f; // of sizePlayer.x
	float minimalStandingFractionConveyorBelt = 0.05f;
	bool upsidedown, actionPressedBeforeCollition, dying;
	int framesSinceDeath;
	bool isStandingOnMovingEntity;
	bool collidingWithWall;
	Entity * standingOn;
	SoundSystemClass sound;
	SoundClass soundSample;
};


#endif // _PLAYER_INCLUDE
