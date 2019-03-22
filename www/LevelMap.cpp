#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "LevelMap.h"
#include "TileMap.h"

using namespace std;

#define LEVEL_DIR string("levels/")
#define LEVEL_MAP_FILE string("levelMap.txt") // TODO to config header

LevelMap::LevelMap()
{
	loadLevelMap(LEVEL_DIR + LEVEL_MAP_FILE);
}

LevelMap::~LevelMap()
{
	if(map != NULL)
		delete map;
}

bool LevelMap::loadLevelMap(const string &fileName){
	ifstream fin;
	istringstream ss;
	string line, cell;

	// open file
	fin.open(fileName.c_str());
	if(!fin.is_open())
		return false;
	// remove comments
	while (! isdigit((fin >> ws).peek())){
		// (check if next char, ignoring whitespace, is a digit)
		getline(fin, line);
	}
	// get map size
	getline(fin, line);
	ss.str(line);
	ss >> size.x >> size.y;
	// create map
	map = new int[size.x * size.y];
	for(int j=0; j<size.y; j++)
	{
		int i = 0;
		getline(fin, line);
		ss.clear(); // needed, otherwise keeps first line
		ss.str(line);
		while(getline(ss, cell, ',')) {
			if (stoi(cell) == 1) // for autoload of level 1
				this->currentLevelMapIdx = j*size.x + i;
			map[j*size.x + i++] = stoi(cell);
		}
	}
}

string LevelMap::nameOfNextLevel(direction nextLevelIs){
	int nextLevelMapIdx = this->currentLevelMapIdx;
	switch (nextLevelIs) {
		case RIGHT:
			nextLevelMapIdx += 1;
			break;
		case LEFT:
			nextLevelMapIdx -= 1;
			break;
		case UP:
			nextLevelMapIdx -= size.x; // one row
			break;
		case DOWN:
			nextLevelMapIdx += size.x; // one row
			break;
	}
	// if outside of array, reset to current
	if (nextLevelMapIdx < 0 || size.x * size.y <= nextLevelMapIdx){
		nextLevelMapIdx = this->currentLevelMapIdx;
	}
	this->currentLevelMapIdx = nextLevelMapIdx;
	return nameOfCurrentLevel();
}

string LevelMap::nameOfCurrentLevel(){
	// get current levelFileID from map
	int currentLevelFileID = map[this->currentLevelMapIdx];
	// get string from levelFileID
	string currentLevelFileID_s = std::to_string(currentLevelFileID);
	if (currentLevelFileID < 10){
		currentLevelFileID_s = "0" + currentLevelFileID_s;
	}
	return LEVEL_DIR + "level" + currentLevelFileID_s + ".tmx";
}

// only use for saving and restoring game
int LevelMap::getCurrentScreen(){
	// abstract from implementation (from the fact, that's only an index)
	return this->currentLevelMapIdx;
}

// only use for saving and restoring game
void LevelMap::setCurrentScreen(int newCurrentScreen){
	this->currentLevelMapIdx = newCurrentScreen;
}
