#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>

class Sprite;
class Texture;
class ShaderProgram;
class BoundingShape;
class AxisAlignedBoundingBox;

// General game object
// has a position, a size and a speed with direction
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
	void setTextureBounds(glm::vec2 textureCoordinates, glm::vec2 textureSize);
	void setPosition(glm::vec2 initPos){ this->position = initPos; }
	glm::vec2 getPosition() const { return this->position; }
	void setSize(glm::vec2 size){ this->size = size; }
	glm::vec2 getSize() const { return this->size; }
	BoundingShape * getBoundingShape() const;
	glm::vec2 getVelocity() const;
	void setDirection(int xDirection);
	void setID(int ID){ this->ID = ID; }
	int getID(){ return this->ID; }
	void setTileID(int tileID) { this->tileID = tileID; }
	int getTileID() { return this->tileID; }
	void setNumberAnimations(int numberAnimations);
	void addAnimation(glm::vec2 newTextureCoords);

	virtual void handleCollision(Entity & e) = 0;

protected:
	// physical
	glm::vec2 position; // top left
	glm::vec2 size;
	float speed; // TODO change to glm::vec2
	glm::vec2 direction = glm::vec2(1.f, 0.f); // moving direction (length 1)
	// collision
	BoundingShape * collisionBounds;
	AxisAlignedBoundingBox * defaultCollisionBox;
	// tiled
	int ID; // like in object layer of level
	int tileID; // index in stylesheet
	// quad & texture
	Sprite * sprite;
	glm::vec2 textureCoordinates, textureSize; // for tilesheet
	int animationCount = 0;

};

#endif // _ENTITY_INCLUDE
