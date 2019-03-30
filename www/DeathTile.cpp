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

void DeathTile::changeTexture(glm::vec2 newTextureCoords){
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, newTextureCoords);
	sprite->changeAnimation(0);
}
