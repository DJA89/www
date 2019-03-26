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
}

void Entity::update(int deltaTime){
	// move
	position += velocity * glm::normalize(direction);
	// update sprite and bounds
	sprite->setPosition(position);
	getBoundingShape()->setPosition(position);
	sprite->update(deltaTime);
}

void Entity::render(){
	sprite->render();
}

BoundingShape * Entity::getBoundingShape(){
	if (this->collisionBounds != NULL){
		return this->collisionBounds;
	} else {
		// set default collision path
		if (defaultCollisionBox == NULL){
			defaultCollisionBox = new AxisAlignedBoundingBox(this->position, this->size);
		}
		return this->defaultCollisionBox;
	}
}
