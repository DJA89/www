#ifndef _CHECKPOINT_INCLUDE
#define _CHECKPOINT_INCLUDE

#include <glm/glm.hpp>
#include "Entity.h"

class Sprite;
class Texture;
class ShaderProgram;

// Checkpoint object
// has a position, a size
// has a boundingShape (used for collision detection)

class Checkpoint : public Entity
{

public:
	void init(Texture & tilesheet, ShaderProgram & shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileID(int tileID) { this->tileID = tileID; }
	int getTileID() { return this->tileID; }
	void handleCollision(Entity & e);

private:
	int tileID; // index in stylesheet

};

#endif // _CHECKPOINT_INCLUDE
