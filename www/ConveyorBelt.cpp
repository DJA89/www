#include "ConveyorBelt.h"

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


void ConveyorBelt::setAboveVelocity(float velocity) {
	aboveVelocity = velocity;
}

void ConveyorBelt::setBelowVelocity(float velocity) {
	belowVelocity = velocity;
}
