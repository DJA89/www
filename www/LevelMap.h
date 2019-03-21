#ifndef _LEVEL_MAP_INCLUDE
#define _LEVEL_MAP_INCLUDE

#include <glm/glm.hpp>
#include "TileMap.h"

using namespace std;

// Class LevelMap manages the layout of the game levels

enum direction { RIGHT, LEFT, UP, DOWN }; // TODO move to common place

class LevelMap
{

public:
	LevelMap();
	~LevelMap();
	string nameOfNextLevel(direction nextLevelIs);

private:
	bool loadLevelMap(const string &fileName);

public:
	int currentLevelMapIdx; // for map array

private:
	glm::ivec2 size;
	int *map;
};


#endif // _LEVEL_MAP_INCLUDE