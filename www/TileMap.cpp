#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "lib/tinyxml2.h"

using namespace std;
namespace xml = tinyxml2;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string & levelFile){
	// call different functions for tmx and normal (txt) map files
	if (levelFile.rfind(".tmx") == (levelFile.size()-4)) {
		return TileMap::loadLevelTmx(levelFile);
	} else {
		return TileMap::loadLevelTxt(levelFile);
	}
}

bool TileMap::loadLevelTmx(const string &levelFile){
	// Structure of the XML file:
	// <?xml ... ?>
	// <map ... width="20" height="15" tilewidth="16" ...>
	//   <tileset ... />
	//   <layer ... >
	//     <data ... >
	//       0,1,0,1,... // data
	//     </data>
	//   </layer>
	// </map>

	xml::XMLDocument mapTmx;
	mapTmx.LoadFile(levelFile.c_str());

	// get metadata
	const xml::XMLElement* mapConf = mapTmx.FirstChildElement("map");
	mapSize.x = atoi(mapConf->Attribute("width"));
	mapSize.y = atoi(mapConf->Attribute("height"));
	tileSize = atoi(mapConf->Attribute("tilewidth"));
	blockSize = tileSize; // TODO differentiate???

	// get tilesheet name and config
	string tileSetTmxName = mapConf->FirstChildElement("tileset")->Attribute("source");
	xml::XMLDocument tileSetTmx;
	tileSetTmx.LoadFile((LEVEL_DIR + tileSetTmxName).c_str());
	const xml::XMLElement* tileSetConf = tileSetTmx.FirstChildElement("tileset");
	tileSetTmx.SaveFile("bla.xml");
	tilesheetSize.x = atoi(tileSetConf->Attribute("columns"));
	tilesheetSize.y = atoi(tileSetConf->Attribute("tilecount")) / tilesheetSize.x;
	string tilesheetFile = tileSetConf->FirstChildElement("image")->Attribute("source");
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	// load tilesheet file
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	// extract map data
	istringstream tiles (mapConf->FirstChildElement("layer")->FirstChildElement("data")->GetText());
	map = new int[mapSize.x * mapSize.y];
	string row;
	getline(tiles, row); // skip empty line
	for (int j = 0; j < mapSize.y; j++){
		getline(tiles, row); // for each row
		istringstream ss(row);
		for (int i = 0; i < mapSize.x; i++){
			string tileID;
			getline(ss, tileID, ','); // split into tiles
			if (isNumber(tileID)){
				// INFO: 0 means empty, texture tiles start with 1
				map[j*mapSize.x + i] = stoi(tileID);
			} else {
			}
		}
	}

	return true;
}

bool TileMap::isNumber(const string &toCheck){
	return !toCheck.empty() && toCheck.find_first_not_of("-0123456789") == std::string::npos;
}

bool TileMap::loadLevelTxt(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	string tile;

	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		int i = 0;
		getline(fin, line);
		istringstream ss(line);
		string token;
		while(getline(ss, tile, ',')) {
			if (tile[0] == '\r' || tile[0] == '\n'){
				;
			} else if (tile ==  " "){
				map[j*mapSize.x+i++] = 0;
			} else {
				map[j*mapSize.x+i++] = stoi(tile);
			}
		}
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			if(tileSize * y + size.y - *posY <= 4)
			{
				*posY = tileSize * y;
			}
			return true;
		}
	}

	return false;
}
