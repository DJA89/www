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
	levelMap = new LevelMap(); // autoloads level 1
	string mapName = LEVEL_DIR + "level01.tmx";
	map = TileMap::createTileMap(mapName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
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
	checkForCheckpointCollision();
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

void Scene::checkForCheckpointCollision(){
	// TODO store checkpoint in checkpoint class (or TileMap class)
	// TODO => move saving to saveGame() and call if collision
	glm::ivec2 playerPosition = player->posPlayer;
	glm::ivec2 playerSize = player->sizePlayer;
	bool upsidedown = player->upsidedown;
	glm::ivec2 checkpointPosition =
		map->returnCheckPointIfCollision(playerPosition, playerSize, upsidedown);
	if (checkpointPosition != glm::ivec2(0, 0)){ // (0,0) means no collision
		saveGame(
			map->getNormalizedCheckpointPosition(checkpointPosition),
			map->isCheckpointUpsideDown(checkpointPosition));
	}
}

// completely saves game (fully restoreable)
// normalized: the player will be spawned shifted left by half its length
void Scene::saveGame(glm::ivec2 normalizedPosition, bool isUpsideDown){
	savedState.update(normalizedPosition, isUpsideDown);
}

// completely restores game to last save
void Scene::loadGame(){
	glm::ivec2 normalizedPosition = savedState.getSavedPosPlayer();
	bool upsidedown = savedState.getSavedUpsideDown();
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
