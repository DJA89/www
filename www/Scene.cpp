#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "LevelMap.h"

// border left and up of tilemap
#define SCREEN_X 32
#define SCREEN_Y 16

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
	if(levelMap != NULL)
		delete levelMap;
}


void Scene::init()
{
	initShaders();
	levelMap = new LevelMap(); // autoloads level 1
	string mapName = LEVEL_DIR + "level01.tmx";
	map = TileMap::createTileMap(mapName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	saveGame();
	projection = glm::ortho(32.f, float(SCREEN_WIDTH/2 - 1 + 32), float(SCREEN_HEIGHT/2 - 1), 32.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	// update player
	if(player->hasDied()){
		loadGame();
	}
	player->update(deltaTime);
	player->checkForCheckpointCollision(savedState);
	// update tilemap
	glm::ivec2 maxPos = glm::ivec2(map->mapSize.x, map->mapSize.y) * map->getTileSize();
	// if player left level => change level and wrap player position around
	if (player->posPlayer.x + player->sizePlayer.x >= maxPos.x){
		string nextLevelName = levelMap->nameOfNextLevel(RIGHT);
		map = TileMap::createTileMap(nextLevelName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setTileMap(map);
		player->posPlayer.x = 0;
	} else if (player->posPlayer.x < 0){
		string nextLevelName = levelMap->nameOfNextLevel(LEFT);
		map = TileMap::createTileMap(nextLevelName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setTileMap(map);
		player->posPlayer.x = maxPos.x - player->sizePlayer.x;
	} else if (player->posPlayer.y + player->sizePlayer.y >= maxPos.y){
		string nextLevelName = levelMap->nameOfNextLevel(DOWN);
		map = TileMap::createTileMap(nextLevelName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setTileMap(map);
		player->posPlayer.y = 0;
	} else if (player->posPlayer.y < 0){
		string nextLevelName = levelMap->nameOfNextLevel(UP);
		map = TileMap::createTileMap(nextLevelName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setTileMap(map);
		player->posPlayer.y = maxPos.y - player->sizePlayer.y;
	}
}

// completely saves game (fully restoreable)
void Scene::saveGame(){
	player->initializeSavedState(savedState);
}

// completely restores game to last save
void Scene::loadGame(){
	player->loadState(savedState);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
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
