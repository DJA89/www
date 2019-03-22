#include "SavedState.h"

void SavedState::save(glm::ivec2 newSavedPlayerPosition, bool newSavedUpsideDown, int newSavedScreen)
{
	this->savedPlayerPosition = newSavedPlayerPosition;
	this->savedUpsideDown = newSavedUpsideDown;
	this->savedScreen = newSavedScreen;
}

glm::ivec2 SavedState::getSavedPlayerPosition() {
	return this->savedPlayerPosition;
}

bool SavedState::getSavedUpsideDown() {
	return this->savedUpsideDown;
}

int SavedState::getSavedScreen(){
	return this->savedScreen;
}
