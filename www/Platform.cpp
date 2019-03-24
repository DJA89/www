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
	// setup sprite
	sprite = Sprite::createSprite(this->size, this->size, &tilesheet, &shaderProgram);
	sprite->setPosition(position);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
	// correct endpoint so it matches the upper left corner of the platform (and not the bottom right)
	pathEnd = pathEnd - size; // TODO use collision box to normalize path
}

void Platform::update(int deltaTime){
	sprite->update(deltaTime);
	// glm::vec2 direction = pathEnd - pathStart;
	// cout << direction.y << endl;
	// position += velocity * glm::normalize(direction);
	position += velocity * glm::vec2(1, 0); // TODO change to dynamic
	float pathLength = glm::distance(pathStart, pathEnd);
	if (glm::distance(position, pathEnd) > pathLength){
		// we passed pathStart
		position = 2.f * pathStart - position; // mirror on pathStart
		velocity = -velocity;
	} else if (glm::distance(position, pathStart) > pathLength){
		// we passed pathEnd
		position = 2.f * pathEnd - position; // mirror on pathEnd
		velocity = -velocity;
	}
	sprite->setPosition(position);
}

void Platform::render(){
	sprite->render();
}
