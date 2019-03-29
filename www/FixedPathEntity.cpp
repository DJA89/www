#include "FixedPathEntity.h"
#include "BoundingShape.h"
#include "Sprite.h"

void FixedPathEntity::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	Entity::init(tilesheet, shaderProgram);
	// correct endpoint so it matches the upper left corner of the platform (and not the bottom right)
	this->pathEnd = this->pathEnd - this->size; // TODO use collision box to normalize path
	// velocity and direction
	velocity = 1.f;
	direction = pathEnd - pathStart; // to right
}

void FixedPathEntity::setEnemy(bool amEnemy) {
	ImEnemy = amEnemy;
}

void FixedPathEntity::update(int deltaTime){
	// move
	position += velocity * glm::normalize(direction);
	// change direction and fix position if left path
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
	// update sprite and bounds
	sprite->setPosition(position);
	this->getBoundingShape()->recalculateFromEntityPosition(position);
	sprite->update(deltaTime);
}

void FixedPathEntity::render(){
	Entity::render();
}

void FixedPathEntity::setSpawn(glm::vec2 initPos){
	// Entity::position = initPos;
	this->position = initPos;
}

void FixedPathEntity::setSize(glm::vec2 size){
	// Entity::size = size;
	this->size = size;
}

void FixedPathEntity::handleCollision(Entity & e) {
	e.handleCollision(*this);
}
