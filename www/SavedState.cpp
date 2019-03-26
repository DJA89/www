#include "SavedState.h"


void SavedState::save(glm::ivec2 newSavedPlayerPosition, bool newSavedUpsideDown, int newSavedScreen)
{
	this->savedPlayerPosition = newSavedPlayerPosition;
	this->savedUpsideDown = newSavedUpsideDown;
	this->savedScreen = newSavedScreen;
}
