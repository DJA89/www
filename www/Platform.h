#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE


#include "Sprite.h"
class BoundingShape;
class AxisAlignedBoundingBox;

// Platform is a Sprite that represents a moving platform in game. As such it has
// all properties it needs to track its path, movement, carrying the player and collisions.


class Platform
{

public:
	~Platform();
	void init(Texture & tilesheet, ShaderProgram & shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileID(int tileID){ this->tileID = tileID; }
	void setID(int ID){ this->ID = ID; }
	int getID(){ return this->ID; }
	void setSpawn(glm::vec2 initPos){ this->position = initPos; }
	void setSize(glm::vec2 size){ this->size = size; }
	void setPathStart(glm::vec2 pathStart){ this->pathStart = pathStart; }
	void setPathEnd(glm::vec2 pathEnd){ this->pathEnd = pathEnd; } // gets corrected in init() to match upper left corner
	void setBoundingShape(BoundingShape * collisionBounds){ this->collisionBounds = collisionBounds; }
	BoundingShape * getBoundingShape();
	void setTextureBounds(glm::vec2 textureCoordinates, glm::vec2 textureSize){ this->textureCoordinates = textureCoordinates; this->textureSize = textureSize; }
	float getVelocity() const { return velocity; }


private:
	Texture tilesheet;
	glm::vec2 textureCoordinates, textureSize; // for tilesheet
	Sprite * sprite;
	float velocity = 1.f;

	int ID; // like in object layer of level
	int tileID; // index in stylesheet
	glm::vec2 size;
	glm::vec2 position; // bottom left
	glm::vec2 pathStart;
	glm::vec2 pathEnd; // in init gets subtracted size (then matches path of bottom left corner)
	BoundingShape * collisionBounds;
	AxisAlignedBoundingBox * defaultCollisionBox;

};

#endif // _PLATFORM_INCLUDE
