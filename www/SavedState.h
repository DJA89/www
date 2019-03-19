#ifndef _SAVED_STATE_INCLUDE
#define _SAVED_STATE_INCLUDE

#include <glm/glm.hpp>

class TileMap;

class SavedState
{

public:	
	void init(glm::ivec2 savedTileMapDispl, glm::ivec2 savedPosPlayer, bool savedUpsideDown);
	void update(glm::ivec2 newSavedTileMapDispl, glm::ivec2 newSavedPosPlayer, bool newSavedUpsideDown);
	glm::ivec2 getSavedTileMapDispl(); 
	glm::ivec2 getSavedPosPlayer();
	bool getSavedUpsideDown();

private:
	glm::ivec2 savedTileMapDispl;
	glm::ivec2 savedPosPlayer;
	bool savedUpsideDown;
};


#endif // _SAVED_STATE_INCLUDE