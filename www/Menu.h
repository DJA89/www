#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include "Sprite.h"


class Menu
{
public:
	void init(ShaderProgram &shaderProgram);
	int update();
	void render();
	enum selectedOption
	{
		MAIN_GAME, HOW_TO, CREDITS, HOW_TO_SELECTED, CREDITS_SELECTED, MAIN_MENU
	};

private:
	Texture spritesheet;
	glm::ivec2 sizeMenu;
	Sprite *sprite;
	int currentSelected;
	int leftFrames;
	int rightFrames;
	int enterFrames;

private:
	int LeftSelect();
	int RightSelect();
};

#endif // _MENU_INCLUDE
