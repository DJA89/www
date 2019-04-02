#include "SavedState.h"


void SavedState::save(glm::vec2 newSavedPlayerPosition, bool newSavedUpsideDown, int newSavedScreen)
{
	this->savedPlayerPosition = newSavedPlayerPosition;
	this->savedUpsideDown = newSavedUpsideDown;
	this->savedScreen = newSavedScreen;
}
