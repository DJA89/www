#ifndef _FIXED_PATH_ENTITY_INCLUDE
#define _FIXED_PATH_ENTITY_INCLUDE

#include <glm/glm.hpp>
#include "Entity.h"

class Sprite;
class Texture;
class ShaderProgram;
class BoundingShape;
class AxisAlignedBoundingBox;

// General game object
// has a position, a size and a velocity
// has a boundingShape (used for collision detection)
// has a method to handle collisions

class FixedPathEntity: public Entity
{

public:
	void init(Texture & tilesheet, ShaderProgram & shaderProgram, bool ImEnemy = false);
	void update(int deltaTime);
	void render();
	void setSpawn(glm::vec2 initPos);
	void setSize(glm::vec2 size);
	void setPathStart(glm::vec2 pathStart){ this->pathStart = pathStart; }
	void setPathEnd(glm::vec2 pathEnd){ this->pathEnd = pathEnd; }

	void setTileID(int tileID) { this->tileID = tileID; }
	void handleCollision(Entity & e);
	bool IsEnemy() { return this->ImEnemy; }

protected:
	glm::vec2 pathStart;
	glm::vec2 pathEnd; // in init gets subtracted size (then matches path of bottom left corner)

private:
	int tileID; // index in stylesheet
	bool ImEnemy;

};

#endif // _FIXED_PATH_ENTITY_INCLUDE
