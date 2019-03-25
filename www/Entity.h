#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>

class Sprite;
class Texture;
class ShaderProgram;
// class Collision;
class BoundingShape;
class AxisAlignedBoundingBox;

// General game object
// has a position, a size and a velocity
// has a boundingShape (used for collision detection)
// has a method to handle collisions

class Entity
{

public:
	~Entity();

	void init(Texture & tilesheet, ShaderProgram & shaderProgram);
	void update(int deltaTime);
	void render();
	void setBoundingShape(BoundingShape * collisionBounds){ this->collisionBounds = collisionBounds; }
	void setTextureBounds(glm::vec2 textureCoordinates, glm::vec2 textureSize){ this->textureCoordinates = textureCoordinates; this->textureSize = textureSize; }
	void setPosition(glm::vec2 initPos){ this->position = initPos; }
	void setSize(glm::vec2 size){ this->size = size; }
	BoundingShape * getBoundingShape();
	float getVelocity() const { return velocity; }
	void setID(int ID){ this->ID = ID; }
	int getID(){ return this->ID; }
	// virtual void handleCollision(Collision c);

protected:
	// physical
	glm::vec2 position; // bottom left
	glm::vec2 size;
	float velocity; // TODO change to glm::vec2
	glm::vec2 direction; // moving direction (length 1)
	// collision
	BoundingShape * collisionBounds;
	AxisAlignedBoundingBox * defaultCollisionBox;
	// tiled
	int ID; // like in object layer of level
	// quad & texture
	Sprite * sprite;
	glm::vec2 textureCoordinates, textureSize; // for tilesheet

};

#endif // _ENTITY_INCLUDE
