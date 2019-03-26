#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "LevelMap.h"


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

private:
	void initShaders();
	void saveGame();
	void loadGame();

public:
	ShaderProgram texProgram;

private:
	LevelMap *levelMap;
	TileMap *map;
	Player *player;
	SavedState savedState;
	float currentTime;
	glm::mat4 projection;
};


#endif // _SCENE_INCLUDE
