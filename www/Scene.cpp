#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "LevelMap.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "Intersection.h"
#include "Checkpoint.h"

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
}


void Scene::init()
{
	initShaders();
	// map
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
	SoundSystemClass sound = SoundSystemClass();

	sound.createSound(&soundSample, "sounds/music.mp3");

	//sound.playSound(soundSample, true);

}

void Scene::loadLevel(string levelName){
	// tilemap
	map = TileMap::createTileMap(levelName, texProgram);
	// platforms
	for (auto it = map->entities.begin(); it != map->entities.end(); ++it){
		Entity * p = it->second;
		p->init(map->tilesheet, texProgram);
	}
	for (auto it = map->checkpoints.begin(); it != map->checkpoints.end(); ++it){
		Checkpoint * c = it->second;
		c->init(map->tilesheet, texProgram);
	}
	for (auto it = map->cbfs.begin(); it != map->cbfs.end(); ++it) {
		ConveyorBelt * cbf = it->second;
		cbf->init(map->tilesheet, texProgram);
	}
}

void Scene::endGame() {
	//sound.releaseSound(soundSample);
	player->endGame();
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	// update player
	if(player->hasDied()){
		loadGame();
	}
	// update all moving entities: platforms, player, ...
	if(!player->isDying()) {
		for (auto it = map->entities.begin(); it != map->entities.end(); ++it) {
			if (dynamic_cast<FixedPathEntity*>(it->second)) {
				dynamic_cast<FixedPathEntity*>(it->second)->update(deltaTime);
			}
		}
		for (auto it = map->checkpoints.begin(); it != map->checkpoints.end(); ++it) {
			it->second->update(deltaTime);
		}
		for (auto it = map->cbfs.begin(); it != map->cbfs.end(); ++it) {
			it->second->update(deltaTime);
		}
	}

	player->update(deltaTime);
	// check for collisions between player and platforms
	// TODO move playerCollisionBounds to player (as pointer variable); later load from xml
	BoundingShape * playerCollisionBounds = new AxisAlignedBoundingBox(glm::vec2(0, 0), player->getSize());
	playerCollisionBounds->recalculateFromEntityPosition(player->getPosition());
	if (!player->isDying()) {
		for (auto it = map->entities.begin(); it != map->entities.end(); ++it) {
			Entity * ent = it->second;
			if (Intersection::check(*(ent->getBoundingShape()), *playerCollisionBounds)) {
				if (dynamic_cast<FixedPathEntity*>(ent)) {
					FixedPathEntity* fpe = dynamic_cast<FixedPathEntity*>(ent);
					if (fpe->IsEnemy()) {
						player->handleCollisionWithDeath(*fpe);
						break;
					}
				}
				player->handleCollisionWithPlatform(*ent);
				// cout << "PLAYER with platform collision" << endl;
			}
		}
		playerCollisionBounds->recalculateFromEntityPosition(player->getPosition());
		for (auto it = map->cbfs.begin(); it != map->cbfs.end(); ++it) {
			ConveyorBelt * cbf = it->second;
			if (Intersection::check(*(cbf->getBoundingShape()), *playerCollisionBounds)) {
				player->handleCollisionWithPlatform(*cbf);
			}
		}
			// checkpoints
		for (auto it = map->checkpoints.begin(); it != map->checkpoints.end(); ++it) {
			Checkpoint * cp = it->second;
			if (Intersection::check(*(cp->getBoundingShape()), *playerCollisionBounds)) {
				handleCheckpointCollision(cp);
			}
		}
	}

	delete playerCollisionBounds;

	// update tilemap
	// if player left level => change level and wrap player position around
	glm::ivec2 maxPos = glm::ivec2(map->mapSize.x, map->mapSize.y) * map->getTileSize();
	if (player->getPosition().x + player->getSize().x >= maxPos.x){
		string nextLevelName = levelMap.nameOfNextLevel(RIGHT);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionX(0);
	} else if (player->getPosition().x < 0){
		string nextLevelName = levelMap.nameOfNextLevel(LEFT);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionX(maxPos.x - player->getSize().x);
	} else if (player->getPosition().y + player->getSize().y >= maxPos.y){
		string nextLevelName = levelMap.nameOfNextLevel(DOWN);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionY(0);
	} else if (player->getPosition().y < 0){
		string nextLevelName = levelMap.nameOfNextLevel(UP);
		loadLevel(nextLevelName);
		player->setTileMap(map);
		player->setPositionY(maxPos.y - player->getSize().y);
	}
}

void Scene::handleCheckpointCollision(Checkpoint * cp){
	// TODO store checkpoint in checkpoint class (or TileMap class)
	// TODO => move saving to saveGame() and call if collision
	// player data
	glm::ivec2 playerPosition = player->getPosition();
	glm::ivec2 playerSize = player->getSize();
	bool upsidedown = player->getIfUpSideDown();
	// checkpoint data
	glm::vec2 checkpointPosition = cp->getPosition();
	glm::vec2 checkpointSize = cp->getSize();
	int checkpointTileID = cp->getTileID();
	// get if checkpoint upsidedown
	bool isCheckpointUpsideDown = (checkpointTileID == 593 || checkpointTileID == 595);
	// compute normalized checkpoint position
	glm::ivec2 normalizedCheckpointPosition;
	if (isCheckpointUpsideDown){
			normalizedCheckpointPosition = glm::ivec2(checkpointPosition.x + checkpointSize.x/2, checkpointPosition.y);
		} else {
			normalizedCheckpointPosition = glm::ivec2(checkpointPosition.x + checkpointSize.x/2, checkpointPosition.y + checkpointSize.y);
		}
	// change checkpoint image from spider web to egg
	if (checkpointTileID == 592 || checkpointTileID == 593){ // spider web
		int newTileID = cp->getTileID() + 2;
		cp->setTileID(newTileID); // spider web => egg
		glm::vec2 newTextureCoords = map->getTextureCoordsForTileID(newTileID);
		cp->changeTexture(newTextureCoords);
		// change last checkpoint back to spider web
		for (auto it = map->checkpoints.begin(); it != map->checkpoints.end(); ++it){
			Checkpoint * other = it->second;
			if (other != cp){
				int otherTileID = other->getTileID();
				if (otherTileID == 594 || otherTileID == 595){ // egg
					// change egg back to spider web
					int newOtherTileID = otherTileID - 2;
					other->setTileID(newOtherTileID); // egg => spider web
					glm::vec2 newOtherTextureCoords = map->getTextureCoordsForTileID(newOtherTileID);
					other->changeTexture(newOtherTextureCoords);
				}
			}
		}
	}
	// TODO move method calls into saveGame
	saveGame(normalizedCheckpointPosition, isCheckpointUpsideDown);
}

// normalized: the player will be spawned shifted left by half its length
void Scene::saveGame(glm::ivec2 normalizedPosition, bool isUpsideDown){
	int currentScreen = levelMap.getCurrentScreen();
	savedState.save(normalizedPosition, isUpsideDown, currentScreen);
}

// completely restores game to last save
void Scene::loadGame(){
	// TODO improvement: store normalizedPosition and upsidedown in a PlayerState class and currentLevel in a LevelState class
	// levelmap
	int currentScreen = savedState.getSavedScreen();
	if (levelMap.getCurrentScreen() != currentScreen) {
		// TODO move tilemap management to levelmap
		levelMap.setCurrentScreen(currentScreen);
		// map
		if (map != NULL) delete map; // destroy current map
		string levelName = levelMap.nameOfCurrentLevel();

		loadLevel(levelName);
	}

	// player related
	player->setTileMap(map);
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
	for (auto it = map->checkpoints.begin(); it != map->checkpoints.end(); ++it){
		it->second->render();
	}
	for (auto it = map->entities.begin(); it != map->entities.end(); ++it){
		it->second->render();
	}
	for (auto it = map->cbfs.begin(); it != map->cbfs.end(); ++it) {
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
