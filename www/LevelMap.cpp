#include <iostream>
#include <fstream>
#include <sstream>
#include "LevelMap.h"

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
			map[j*size.x + i++] = stoi(cell);
		}
	}
}
