#include "SavedState.h"



void SavedState::init(glm::ivec2 savedTileMapDispl, glm::ivec2 savedPosPlayer, bool savedUpsideDown)
{
	this->savedPosPlayer = savedPosPlayer;
	this->savedTileMapDispl = savedTileMapDispl;
	this->savedUpsideDown = savedUpsideDown;
}

void SavedState::update(glm::ivec2 newSavedTileMapDispl, glm::ivec2 newSavedPosPlayer, bool newSavedUpsideDown)
{
	savedPosPlayer = newSavedPosPlayer;
	savedTileMapDispl = newSavedTileMapDispl;
	savedUpsideDown = newSavedUpsideDown;
}

glm::ivec2 SavedState::getSavedTileMapDispl() {
	return savedTileMapDispl;
}
glm::ivec2 SavedState::getSavedPosPlayer() {
	return savedPosPlayer;
}
bool SavedState::getSavedUpsideDown() {
	return savedUpsideDown;
}
