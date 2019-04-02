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

private:
	int tileID; // index in stylesheet
};

#endif // _CONVEYOR_BELT_INCLUDE
