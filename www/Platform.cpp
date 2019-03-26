#include <iostream>
#include "Platform.h"

void Platform::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	FixedPathEntity::init(tilesheet, shaderProgram);
	// velocity and direction
	velocity = 1.f;
	// TODO glm::vec2 direction = pathEnd - pathStart;
	direction = glm::vec2(1, 0); // to right
}

void Platform::update(int deltaTime){
	FixedPathEntity::update(deltaTime);
}

void Platform::render(){
	FixedPathEntity::render();
}

void Platform::handleCollision(Entity & e){
	e.handleCollision(*this);
}
