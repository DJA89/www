#include "Entity.h"
#include "AxisAlignedBoundingBox.h"
#include "Sprite.h"

Entity::~Entity(){
	if (sprite != NULL){
		delete sprite;
	}
	if (defaultCollisionBox != NULL){
		delete defaultCollisionBox;
	}
}

void Entity::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	sprite = Sprite::createSprite(this->size, textureSize, &tilesheet, &shaderProgram);
	sprite->setPosition(position);
	// add default animation (static texture) => override if needed
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, textureCoordinates);
	sprite->changeAnimation(0);
	// velocity and direction
	velocity = 0;
	direction = glm::vec2(1, 0); // to right
	// default bounding box
	defaultCollisionBox = new AxisAlignedBoundingBox(glm::vec2(0, 0), this->size);
	getBoundingShape()->recalculateFromEntityPosition(this->position);
}

void Entity::update(int deltaTime){
	// move
	position += velocity * glm::normalize(direction);
	// update sprite and bounds
	sprite->setPosition(position);
	getBoundingShape()->recalculateFromEntityPosition(position);
	sprite->update(deltaTime);
}

void Entity::render(){
	sprite->render();
}

BoundingShape * Entity::getBoundingShape() const {
	if (this->collisionBounds != NULL){
		return this->collisionBounds;
	} else {
		return this->defaultCollisionBox;
	}
}
