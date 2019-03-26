#ifndef _SAVED_STATE_INCLUDE
#define _SAVED_STATE_INCLUDE

#include <glm/glm.hpp>

class TileMap;

class SavedState
{

public:
	void save(glm::ivec2 newSavedPlayerPosition, bool newSavedUpsideDown, int newSavedScreen);
	glm::ivec2 getSavedPlayerPosition(){ return savedPlayerPosition; };
	bool getSavedUpsideDown(){ return savedUpsideDown; };
	int getSavedScreen(){ return this->savedScreen; };

private:
	glm::ivec2 savedPlayerPosition;
	bool savedUpsideDown;
	int savedScreen;
};


#endif // _SAVED_STATE_INCLUDE
