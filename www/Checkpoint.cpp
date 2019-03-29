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
