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
#define INIT_PLAYER_X_TILES 2
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
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player->initializeSavedState();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH/2 - 1), float(SCREEN_HEIGHT/2 - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::loadLevel(string levelName){
	// tilemap
	map = TileMap::createTileMap(levelName, glm::vec2(0, 0), texProgram);
	// platforms
	for (auto it = map->platforms.begin(); it != map->platforms.end(); ++it){
		Platform * p = it->second;
		p->init(map->tilesheet, texProgram);
	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	// update all moving entities: platforms, player, ...
	for (auto it = map->platforms.begin(); it != map->platforms.end(); ++it){
		it->second->update(deltaTime);
	}
	player->update(deltaTime);
	// check for collisions between player and platforms
	// TODO move playerCollisionBounds to player (as pointer variable); later load from xml
	BoundingShape * playerCollisionBounds = new AxisAlignedBoundingBox(player->posPlayer, player->sizePlayer);
	for (auto it = map->platforms.begin(); it != map->platforms.end(); ++it){
		Platform * plat = it->second;
		if (Intersection::check(*(plat->getBoundingShape()), *playerCollisionBounds)){
			player->handleCollisionWithPlatform(*plat);
			// cout << "PLAYER with platform collision" << endl;
		}
	}
	delete playerCollisionBounds;

	// if player left level => change level and wrap player position around
	glm::ivec2 maxPos = glm::ivec2(map->mapSize.x, map->mapSize.y) * map->getTileSize();
	if (player->posPlayer.x + player->sizePlayer.x >= maxPos.x){
		string nextLevelName = levelMap->nameOfNextLevel(RIGHT);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->posPlayer.x = 0;
	} else if (player->posPlayer.x < 0){
		string nextLevelName = levelMap->nameOfNextLevel(LEFT);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->posPlayer.x = maxPos.x - player->sizePlayer.x;
	} else if (player->posPlayer.y + player->sizePlayer.y >= maxPos.y){
		string nextLevelName = levelMap->nameOfNextLevel(DOWN);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->posPlayer.y = 0;
	} else if (player->posPlayer.y < 0){
		string nextLevelName = levelMap->nameOfNextLevel(UP);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->posPlayer.y = maxPos.y - player->sizePlayer.y;
	}
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
