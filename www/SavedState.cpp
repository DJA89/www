#include "SavedState.h"



void SavedState::init(glm::ivec2 savedPosPlayer, bool savedUpsideDown)
{
	this->savedPosPlayer = savedPosPlayer;
	this->savedUpsideDown = savedUpsideDown;
}

void SavedState::update(glm::ivec2 newSavedPosPlayer, bool newSavedUpsideDown)
{
	savedPosPlayer = newSavedPosPlayer;
	savedUpsideDown = newSavedUpsideDown;
}

glm::ivec2 SavedState::getSavedPosPlayer() {
	return savedPosPlayer;
}
bool SavedState::getSavedUpsideDown() {
	return savedUpsideDown;
}
