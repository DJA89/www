#ifndef _LEVEL_MAP_INCLUDE
#define _LEVEL_MAP_INCLUDE

#include <glm/glm.hpp>

using namespace std;

// Class LevelMap manages the layout of the game levels

class LevelMap
{

public:
	LevelMap();
	~LevelMap();

private:
	bool loadLevelMap(const string &fileName);

private:
	glm::ivec2 size;
	int *map;
};


#endif // _LEVEL_MAP_INCLUDE
