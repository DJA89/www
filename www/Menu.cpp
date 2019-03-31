#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>

#define SPACEBAR 32
#define RETURN 13
#define ESCAPE 27

void Menu::init(ShaderProgram &shaderProgram)
{
	currentSelected = MAIN_GAME;
	leftFrames = 0;
	rightFrames = 0;
	enterFrames = 0;
	spritesheet.loadFromFile("images/mainMenu2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizeMenu = glm::vec2(1280, 960);
	sprite = Sprite::createSprite(sizeMenu, glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(MAIN_GAME, 8);
	sprite->addKeyframe(MAIN_GAME, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(HOW_TO, 8);
	sprite->addKeyframe(HOW_TO, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(CREDITS, 8);
	sprite->addKeyframe(CREDITS, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(HOW_TO_SELECTED, 8);
	sprite->addKeyframe(HOW_TO_SELECTED, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(CREDITS_SELECTED, 8);
	sprite->addKeyframe(CREDITS_SELECTED, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(currentSelected);
	sprite->setPosition(glm::vec2(0, 0));
}

int Menu::update() {
	bool enterPressed = Game::instance().getKey(SPACEBAR) || Game::instance().getKey(RETURN);
	if (!enterPressed) {
		enterFrames = 0;
	}
	if (currentSelected == CREDITS_SELECTED || currentSelected == HOW_TO_SELECTED) {
		if (enterPressed) {
			if (enterFrames == 0) {
				currentSelected = MAIN_GAME;
				sprite->changeAnimation(MAIN_GAME);
			}
			enterFrames = (enterFrames + 1) % 20;
		}
	}
	else {
		if (enterPressed) {
			if (enterFrames == 0) {
				switch (currentSelected)
				{
				case MAIN_GAME:
					enterFrames = 1;
					return currentSelected;
					break;
				case CREDITS:
					sprite->changeAnimation(CREDITS_SELECTED);
					currentSelected = CREDITS_SELECTED;
					break;
				case HOW_TO:
					sprite->changeAnimation(HOW_TO_SELECTED);
					currentSelected = HOW_TO_SELECTED;
					break;
				}
			}
			enterFrames = (enterFrames + 1) % 20;
		}

		bool moveLeft = Game::instance().getSpecialKey(GLUT_KEY_LEFT) || Game::instance().getSpecialKey(GLUT_KEY_UP);
		bool moveRight = Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getSpecialKey(GLUT_KEY_DOWN);
		if (moveLeft) {
			if (leftFrames == 0) {
				sprite->changeAnimation(LeftSelect());
			}
			leftFrames = (leftFrames + 1) % 20;
		}
		else {
			leftFrames = 0;
		}
		if (moveRight) {
			if (rightFrames == 0 && !moveLeft) {
				sprite->changeAnimation(RightSelect());
			}
			rightFrames = (rightFrames + 1) % 20;
		}
		else {
			rightFrames = 0;
		}
	}
	return MAIN_MENU;
}

int Menu::LeftSelect() {
	currentSelected -= 1;
	if (currentSelected == -1) {
		currentSelected = CREDITS;
	}
	return currentSelected;
}

int Menu::RightSelect() {
	currentSelected += 1;
	if (currentSelected == CREDITS + 1) {
		currentSelected = MAIN_GAME;
	}
	return currentSelected;
}

void Menu::render() {
	sprite->render();
}
