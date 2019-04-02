#ifndef _CONVEYOR_BELT_INCLUDE
#define _CONVEYOR_BELT_INCLUDE

#include "Entity.h"
class ConveyorBelt : public Entity
{
public:
	void init(Texture & tilesheet, ShaderProgram & shaderProgram);
	void update(int deltaTime);
	void handleCollision(Entity & e);
	void setTileID(int tileID) { this->tileID = tileID; }
	void setPosition(glm::vec2 initPos);
	float getVelocity(bool isBelow) const;
	void setDirection(int xDirection);
	void setAboveVelocity(float velocity);
	void setBelowVelocity(float velocity);

private:
	int tileID; // index in stylesheet
	float aboveVelocity;
	float belowVelocity;
};

#endif // _CONVEYOR_BELT_INCLUDE
