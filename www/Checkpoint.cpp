#include "Checkpoint.h"
#include "Sprite.h"

void Checkpoint::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	Entity::init(tilesheet, shaderProgram);
}

void Checkpoint::update(int deltaTime){
	sprite->update(deltaTime);
}

void Checkpoint::render(){
	sprite->render();
}

void Checkpoint::handleCollision(Entity & e) {
	e.handleCollision(*this);
}

void Checkpoint::changeTexture(glm::vec2 newTextureCoords){
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, newTextureCoords);
	sprite->changeAnimation(0);
}
