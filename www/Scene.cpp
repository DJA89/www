#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "LevelMap.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "Intersection.h"
#include "Platform.h"

// initial player position
#define INIT_PLAYER_X_TILES 3.5 // spawn on first checkpoint
#define INIT_PLAYER_Y_TILES 10


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if(levelMap != NULL)
		delete levelMap;
}


void Scene::init()
{
	initShaders();
	// map
	levelMap = new LevelMap(); // autoloads level 1
	string mapName = LEVEL_DIR + "level01.tmx";
	loadLevel(mapName);
	// player
	player = new Player();
	player->init(texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	// saveGame(); // TODO set initialPos & checkpoint in tiled => load in TileMap
	projection = glm::ortho(0.f, float(SCREEN_WIDTH/2 - 1), float(SCREEN_HEIGHT/2 - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::loadLevel(string levelName){
	// tilemap
	map = TileMap::createTileMap(levelName, texProgram);
	// platforms
	for (auto it = map->platforms.begin(); it != map->platforms.end(); ++it){
		Platform * p = it->second;
		p->init(map->tilesheet, texProgram);
	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	// update player
	if(player->hasDied()){
		loadGame();
	}
	// update all moving entities: platforms, player, ...
	for (auto it = map->platforms.begin(); it != map->platforms.end(); ++it){
		it->second->update(deltaTime);
	}
	player->update(deltaTime);
	checkForCheckpointCollision();
	// check for collisions between player and platforms
	// TODO move playerCollisionBounds to player (as pointer variable); later load from xml
	BoundingShape * playerCollisionBounds = new AxisAlignedBoundingBox(player->getPosition(), player->getSize());
	for (auto it = map->platforms.begin(); it != map->platforms.end(); ++it){
		Platform * plat = it->second;
		if (Intersection::check(*(plat->getBoundingShape()), *playerCollisionBounds)){
			player->handleCollisionWithPlatform(*plat);
			// cout << "PLAYER with platform collision" << endl;
		}
	}
	delete playerCollisionBounds;

	// update tilemap
	// if player left level => change level and wrap player position around
	glm::ivec2 maxPos = glm::ivec2(map->mapSize.x, map->mapSize.y) * map->getTileSize();
	if (player->getPosition().x + player->getSize().x >= maxPos.x){
		string nextLevelName = levelMap->nameOfNextLevel(RIGHT);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionX(0);
	} else if (player->getPosition().x < 0){
		string nextLevelName = levelMap->nameOfNextLevel(LEFT);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionX(maxPos.x - player->getSize().x);
	} else if (player->getPosition().y + player->getSize().y >= maxPos.y){
		string nextLevelName = levelMap->nameOfNextLevel(DOWN);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionY(0);
	} else if (player->getPosition().y < 0){
		string nextLevelName = levelMap->nameOfNextLevel(UP);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionY(maxPos.y - player->getSize().y);
	}
}

void Scene::checkForCheckpointCollision(){
	// TODO store checkpoint in checkpoint class (or TileMap class)
	// TODO => move saving to saveGame() and call if collision
	glm::ivec2 playerPosition = player->getPosition();
	glm::ivec2 playerSize = player->getSize();
	bool upsidedown = player->getIfUpSideDown();
	glm::ivec2 checkpointPosition = map->returnCheckPointIfCollision(playerPosition, playerSize, upsidedown);
	if (checkpointPosition != glm::ivec2(0, 0)){ // (0,0) means no collision
		// TODO move method calls into saveGame
		saveGame(
			map->getNormalizedCheckpointPosition(checkpointPosition),
			map->isCheckpointUpsideDown(checkpointPosition));
	}
}

// normalized: the player will be spawned shifted left by half its length
void Scene::saveGame(glm::ivec2 normalizedPosition, bool isUpsideDown){
	savedState.save(normalizedPosition, isUpsideDown);
}

// completely restores game to last save
void Scene::loadGame(){
	bool upsidedown = savedState.getSavedUpsideDown();
	glm::ivec2 normalizedPosition = savedState.getSavedPlayerPosition();
	player->restorePlayerPosition(upsidedown, normalizedPosition);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f); // TODO remove
	// render map, all platforms and player
	map->render();
	for (auto it = map->platforms.begin(); it != map->platforms.end(); ++it){
		it->second->render();
	}
	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
