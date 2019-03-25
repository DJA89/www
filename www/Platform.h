#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE

#include <glm/glm.hpp>
#include "FixedPathEntity.h"

class Sprite;
class Texture;
class ShaderProgram;
class BoundingShape;
class AxisAlignedBoundingBox;

// Platform is a Sprite that represents a moving platform in game. As such it has
// all properties it needs to track its path, movement, carrying the player and collisions.


class Platform: public FixedPathEntity
{

public:
	void init(Texture & tilesheet, ShaderProgram & shaderProgram);
	// virtual void handleCollision(Collision c);
	void update(int deltaTime);
	void render();

	void setTileID(int tileID){ this->tileID = tileID; }

private:
	int tileID; // index in stylesheet

};

#endif // _PLATFORM_INCLUDE
