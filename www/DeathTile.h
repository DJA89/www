#ifndef _DEATH_TILE_INCLUDE
#define _DEATH_TILE_INCLUDE

#include <glm/glm.hpp>
#include "Entity.h"

class Sprite;
class Texture;
class ShaderProgram;

// DeathTile object
// has a position, a size
// has a boundingShape (used for collision detection)

class DeathTile : public Entity
{

public:
	void init(Texture & tilesheet, ShaderProgram & shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileID(int tileID) { this->tileID = tileID; }
	int getTileID() { return this->tileID; }
	void handleCollision(Entity & e);
	void changeTexture(glm::vec2 newTextureCoords);

private:
	int tileID; // index in stylesheet

};

#endif // _DEATH_TILE_INCLUDE
