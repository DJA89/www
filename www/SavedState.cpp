#include "SavedState.h"

void SavedState::save(glm::ivec2 newSavedPlayerPosition, bool newSavedUpsideDown, int newSavedScreen)
{
	this->savedPlayerPosition = newSavedPlayerPosition;
	this->savedUpsideDown = newSavedUpsideDown;
	this->savedScreen = newSavedScreen;
}

glm::ivec2 SavedState::getSavedPlayerPosition() {
	return savedPlayerPosition;
}

bool SavedState::getSavedUpsideDown() {
	return savedUpsideDown;
}

int SavedState::getSavedScreen(){
	return this->savedScreen;
}
