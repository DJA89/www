#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "LevelMap.h"
#include "SoundSystem.h"
#include "Menu.h"

class Checkpoint;


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void loadLevel(string levelName);
	void addAnimationsToEntity(Entity * e);
	void endGame();

private:
	void initShaders();
	void saveGame(glm::ivec2 normalizedPosition, bool isUpsideDown);
	void loadGame();
	void handleCheckpointCollision(Checkpoint * cp);
	void initMainGame();
	void initMenu();
	void updateMainGame(int deltaTime);
	int updateMenu();
	void renderMainGame();
	void renderMenu();

public:
	ShaderProgram texProgram;

private:
	LevelMap levelMap;
	TileMap *map;
	Player *player;
	Menu *menu;
	SavedState savedState;
	float currentTime;
	glm::mat4 projection;
	SoundSystemClass sound;
	SoundClass soundSample;
	int currentScreen;
	int enterFrames;
};


#endif // _SCENE_INCLUDE
