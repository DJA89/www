#include "DeathTile.h"
#include "Sprite.h"

void DeathTile::init(Texture & tilesheet, ShaderProgram & shaderProgram){
	Entity::init(tilesheet, shaderProgram);
}

void DeathTile::update(int deltaTime){
	sprite->update(deltaTime);
}

void DeathTile::render(){
	sprite->render();
}

void DeathTile::handleCollision(Entity & e) {
	e.handleCollision(*this);
}

void DeathTile::setTextureSize(glm::vec2 textureSize){
	this->textureSize = textureSize;
}
