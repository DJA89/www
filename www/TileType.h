#ifndef _TILETYPE_INCLUDE
#define _TILETYPE_INCLUDE


#include <glm/glm.hpp>
class BoundingShape;

class TileType
{

public:
	TileType(int ID, BoundingShape * collisionBounds) : ID(ID), collisionBounds(collisionBounds) {};
	~TileType();

public:
	// TODO instead of direct access, use in collision function inherited from entity
	BoundingShape * collisionBounds; // only if collidable

private:
	int ID; // like in level file
	bool isDeath; // TODO for future use
	bool isCollidable; // TODO for future use

};


#endif // _TILETYPE_INCLUDE
