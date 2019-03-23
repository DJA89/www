#ifndef _TILETYPE_INCLUDE
#define _TILETYPE_INCLUDE


#include <glm/glm.hpp>

class TileType
{

public:

private:
	int ID; // like in level file
	bool isDeath; // TODO for future use
	bool isCollidable; // TODO for future use
	BoundingShape collisionBounds; // only if collidable

};


#endif // _TILETYPE_INCLUDE
