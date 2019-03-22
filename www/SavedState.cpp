#include "SavedState.h"

void SavedState::save(glm::ivec2 newSavedPlayerPosition, bool newSavedUpsideDown)
{
	savedPlayerPosition = newSavedPlayerPosition;
	savedUpsideDown = newSavedUpsideDown;
}

glm::ivec2 SavedState::getSavedPlayerPosition() {
	return savedPlayerPosition;
}
bool SavedState::getSavedUpsideDown() {
	return savedUpsideDown;
}
