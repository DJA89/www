#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Platform.h"

Platform::~Platform(){
	if (sprite != NULL){
		delete sprite;
	}
}

void Platform::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	sprite = Sprite::createSprite(this->size, this->size, &spritesheet, &shaderProgram);
	sprite->setPosition(initPos);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
}


void Platform::render(){
	sprite->render();
}
