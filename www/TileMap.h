#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <unordered_map>
#include "Texture.h"
#include "ShaderProgram.h"
#include "SavedState.h"
#include "TileType.h"
#include "FixedPathEntity.h"
#include "ConveyorBelt.h"

class Checkpoint;

#define LEVEL_DIR string("levels/")


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, ShaderProgram &program);

	TileMap(const string &levelFile, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool checkpointValid(int xCheckpoint, int yCheckpoint, bool upsidedown) const;
	bool triggerDeath(const glm::ivec2 &pos, const glm::ivec2 &size, bool upsidedown) const;
	glm::vec2 getHalfTexel();
	glm::vec2 getTextureCoordsForTileID(int tileID);
	glm::vec2 getCorrectedTileTextureSize(); // size - halfTexel

private:
	bool loadLevel(const string &levelFile);
	bool loadLevelTmx(const string &levelFile);
	bool loadLevelTxt(const string &levelFile);
	void prepareArrays(ShaderProgram &program);
	static bool isNumber(const string &toCheck);

public:
	glm::ivec2 position, mapSize, tilesheetSize;
	std::unordered_map<int, Entity *> entities;
	std::unordered_map<int, ConveyorBelt *> cbfs;
	std::unordered_map<int, Checkpoint *> checkpoints;
	Texture tilesheet;

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int tileSize, blockSize;
	glm::vec2 tileTexSize;
	int *map;
	std::unordered_map<int, TileType*> tileTypeByID; // ID like in level files
	const static int non_collision_tiles[7];
	const static int death_tiles[2];

};


#endif // _TILE_MAP_INCLUDE
