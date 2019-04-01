#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "Texture.h"
#include "ShaderProgram.h"
#include "SavedState.h"
#include "TileType.h"
#include "FixedPathEntity.h"
#include "ConveyorBelt.h"
#include "DeathTile.h"

class Checkpoint;

#define LEVEL_DIR string("levels/")

// hardcoded tile IDs (like in tilesheet)
#define EMPTY_TILE 0
#define CHECKPOINT_UNSAVED_FLOOR 591
#define CHECKPOINT_UNSAVED_CEILING 592
#define CHECKPOINT_SAVED_FLOOR 593
#define CHECKPOINT_SAVED_CEILING 594


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
	glm::vec2 getHalfTexel();
	glm::vec2 getTextureCoordsForTileID(int tileID);
	glm::vec2 getCorrectedTileTextureSize(); // size - halfTexel

private:
	bool loadLevel(const string &levelFile);
	bool loadLevelTmx(const string &levelFile);
	void prepareArrays(ShaderProgram &program);
	static bool isNumber(const string &toCheck);
	bool tileIsCollidable(int tileID) const;

public:
	glm::ivec2 position, mapSize, tilesheetSize;
	std::unordered_map<int, Entity *> entities;
	std::unordered_map<int, ConveyorBelt *> cbfs;
	std::unordered_map<int, Checkpoint *> checkpoints;
	std::unordered_map<int, vector<int> *> animatedTiles; // tileID -> tileIDs
	vector<DeathTile *> flames;
	Texture tilesheet;

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int tileSize, blockSize;
	glm::vec2 tileTexSize;
	int tileIDOffset; // map is offset with this value
	int *map;
	std::unordered_map<int, TileType*> tileTypeByID; // ID like in level files
	const static int non_collision_tiles[1];

};


#endif // _TILE_MAP_INCLUDE
