#include "TileType.h"
#include "BoundingShape.h"

using namespace std;

TileType::~TileType(){
	if(collisionBounds != NULL)
		delete collisionBounds;
}
