#include "ConveyorBelt.h"
#include "Sprite.h"

void ConveyorBelt::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	Entity::init(tilesheet, shaderProgram);
	// velocity and direction
	velocity = 2.f;
	direction = glm::vec2(1.f, 0.f); // TODO set in TileMap
}

void ConveyorBelt::handleCollision(Entity & e) {
	e.handleCollision(*this);
}

void ConveyorBelt::setPosition(glm::vec2 initPos) {
	// Entity::position = initPos;
	this->position = initPos;
}

float ConveyorBelt::getVelocity(bool isBelow) const {
	if (isBelow) {
		return belowVelocity;
	}
	else {
		return aboveVelocity;
	}
}

void ConveyorBelt::update(int deltaTime){
	// don't move
	sprite->update(deltaTime);
}

void ConveyorBelt::setAboveVelocity(float velocity) {
	aboveVelocity = velocity;
}

void ConveyorBelt::setBelowVelocity(float velocity) {
	belowVelocity = velocity;
}
