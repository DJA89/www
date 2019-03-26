#include "SavedState.h"


void SavedState::save(glm::ivec2 newSavedPlayerPosition, bool newSavedUpsideDown)
{
	savedPlayerPosition = newSavedPlayerPosition;
	savedUpsideDown = newSavedUpsideDown;
}
