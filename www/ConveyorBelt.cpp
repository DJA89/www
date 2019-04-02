#include "ConveyorBelt.h"
#include "Sprite.h"

void ConveyorBelt::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	Entity::init(tilesheet, shaderProgram);
	// set speed for conveyor belts
	speed = 2.f;
}

void ConveyorBelt::handleCollision(Entity & e) {
	e.handleCollision(*this);
}

void ConveyorBelt::update(int deltaTime){
	// don't move
	sprite->update(deltaTime);
}
