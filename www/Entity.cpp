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

void Entity::setTextureBounds(glm::vec2 textureCoordinates, glm::vec2 textureSize){
	this->textureCoordinates = textureCoordinates;
	this->textureSize = textureSize;
}

void Entity::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	sprite = Sprite::createSprite(this->size, textureSize, &tilesheet, &shaderProgram);
	sprite->setPosition(position);
	// add default animation (static texture) => override if needed
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, textureCoordinates);
	sprite->changeAnimation(0);
	// speed (direction has default value in header)
	speed = 0;
	// default bounding box
	defaultCollisionBox = new AxisAlignedBoundingBox(glm::vec2(0, 0), this->size);
	getBoundingShape()->recalculateFromEntityPosition(this->position);
}

void Entity::update(int deltaTime){
	// move
	position += speed * glm::normalize(direction);
	// update sprite and bounds
	sprite->setPosition(position);
	getBoundingShape()->recalculateFromEntityPosition(position);
	sprite->update(deltaTime);
}

void Entity::render(){
	sprite->render();
}

BoundingShape * Entity::getBoundingShape() {
	BoundingShape * currentBS;
	if (this->collisionBounds != NULL){
		currentBS = this->collisionBounds;
	} else if (this->defaultCollisionBox != NULL){
		this->defaultCollisionBox->setSize(this->size); // update size
		currentBS = this->defaultCollisionBox;
	} else { // set new default collision object
		this->defaultCollisionBox = new AxisAlignedBoundingBox(glm::vec2(0, 0), this->size);
		currentBS = this->defaultCollisionBox;
	}
	currentBS->recalculateFromEntityPosition(this->position);
	return currentBS;
}

void Entity::setNumberAnimations(int numberAnimations){
	sprite->setNumberAnimations(numberAnimations);
	animationCount = 0; // see addAnimation
}

void Entity::addAnimation(glm::vec2 newTextureCoords){
	sprite->setAnimationSpeed(this->animationCount, 8);
	sprite->addKeyframe(this->animationCount, newTextureCoords);
	sprite->changeAnimation(this->animationCount);
}

void Entity::setDirection(int xDirection){
	this->direction = glm::vec2(xDirection, 0.f);
}

glm::vec2 Entity::getVelocity() const {
	return speed * glm::normalize(direction);
}

void Entity::setPosition(glm::vec2 initPos) {
	this->position = initPos;
	if (getBoundingShape() != NULL){
		getBoundingShape()->recalculateFromEntityPosition(this->position);
	}
}

void Entity::setTileID(int tileID) {
	this->tileID = tileID;
}
