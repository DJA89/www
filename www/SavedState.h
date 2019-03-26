#ifndef _SAVED_STATE_INCLUDE
#define _SAVED_STATE_INCLUDE

#include <glm/glm.hpp>

class TileMap;

class SavedState
{

public:
	void save(glm::ivec2 newSavedPlayerPosition, bool newSavedUpsideDown);
	glm::ivec2 getSavedPlayerPosition(){ return savedPlayerPosition; };
	bool getSavedUpsideDown(){ return savedUpsideDown; };

private:
	glm::ivec2 savedPlayerPosition;
	bool savedUpsideDown;
};


#endif // _SAVED_STATE_INCLUDE
