#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "TileMap.h"
#include "lib/tinyxml2.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "BoundingEllipse.h"
#include "Utils.h"
#include "Checkpoint.h"

using namespace std;
namespace xml = tinyxml2;
const int TileMap::non_collision_tiles[3] = {0, 140, 465};
const int TileMap::death_tiles[2] = {140, 465};

TileMap *TileMap::createTileMap(const string &levelFile, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, program);
	return map;
}


TileMap::TileMap(const string &levelFile, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;

	// remove all FixedPathEntities
	for (auto it = entities.begin(); it != entities.cend(); ++it ){
		delete it->second;
	}
	entities.clear();
	// remove all checkpoints
	for (auto it = checkpoints.begin(); it != checkpoints.cend(); ++it ){
		delete it->second;
	}
	checkpoints.clear();
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
	// for reference see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/
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
	tilesheetSize.x = atoi(tileSetConf->Attribute("columns"));
	tilesheetSize.y = atoi(tileSetConf->Attribute("tilecount")) / tilesheetSize.x;
	string tilesheetFile = tileSetConf->FirstChildElement("image")->Attribute("source");
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	// load tilesheet file
	tilesheetFile.erase(0, 3); // remove "../" (current path is project, not levels/)
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST); // Pixelated look
	tilesheet.setMagFilter(GL_NEAREST); // Pixelated look

	// load collision bounding-shapes for tiles
	const xml::XMLElement * tile;
	// for each tile found
	tile = tileSetConf->FirstChildElement("tile");
	while (tile){
		// extract of xml
		int tileID = stoi(tile->Attribute("id"));
		const xml::XMLElement * object = tile->FirstChildElement("objectgroup")->FirstChildElement("object");
		int xPos = stoi(object->Attribute("x"));
		int yPos = stoi(object->Attribute("y"));
		int width = stoi(object->Attribute("width"));
		int height = stoi(object->Attribute("height"));
		// store in objects
		glm::vec2 positionInTile = glm::vec2(xPos, yPos); // relative to tile
		glm::vec2 size = glm::vec2(width, height);
		BoundingShape * bs;
		if (object->FirstChildElement("ellipse") != NULL){
			// is an ellipse
			bs = new BoundingEllipse(positionInTile, size);
		} else {
			// is normal rectangle
			bs = new AxisAlignedBoundingBox(positionInTile, size);
		}
		TileType * tileType = new TileType(tileID, bs);
		tileTypeByID[tileID] = tileType;
		// next iteration
		tile = tile->NextSiblingElement("tile");
	}
	tile = NULL; // not needed anymore

	// extract tilemap data
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

	// extract entities in object layer
	const xml::XMLElement* objectgroup = mapConf->FirstChildElement("objectgroup");
	if (objectgroup){
		const xml::XMLElement* object;
		// for each object found
		object = objectgroup->FirstChildElement("object");
		while (object){
			int xPos = stoi(object->Attribute("x"));
			int yPos = stoi(object->Attribute("y"));
			int width = stoi(object->Attribute("width"));
			int height = stoi(object->Attribute("height"));
			if (object->Attribute("name") == NULL){
				// object has no name
				cerr << "ERROR: Object with ID=" << object->Attribute("id") << " in the levelmap has no Name. Objects without name cannot be loaded." << endl;
				object = object->NextSiblingElement("object"); // skip object
				continue;
			}
			string objectName = object->Attribute("name");

			vector<string> objectAttribs = Utils::split(objectName, '_');
			if (objectAttribs.at(0) == "Checkpoint"){
				Checkpoint * newCheckPoint = new Checkpoint();

				int ID = stoi(object->Attribute("id"));
				int tileID = stoi(object->Attribute("gid"));
				newCheckPoint->setTileID(tileID); // tile idx in spritesheet
				// position is bottom left => correct to top left
				// see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object
				newCheckPoint->setPosition(glm::vec2(xPos, yPos - height));
				newCheckPoint->setSize(glm::vec2(width, height));
				// add texture coordinates of tile
				glm::vec2 textureCoords = getTextureCoordsForTileID(tileID);
				newCheckPoint->setTextureBounds(textureCoords, getCorrectedTileTextureSize());
				// add bounding shape to platform
				if (tileTypeByID.count(tileID - 1) == 1){ // -1 because IDs start with 1
					// custom collision bounds (rescaled to fit multi-tile)
					BoundingShape * tileBounds = tileTypeByID[tileID - 1]->collisionBounds;
					BoundingShape * copyTileBounds = tileBounds->clone();
					copyTileBounds->rescale(newCheckPoint->getSize() / float(tileSize));
					newCheckPoint->setBoundingShape(copyTileBounds);
				}
				checkpoints[ID] = newCheckPoint;
			} else if (objectAttribs.at(0) == "Platform" || objectAttribs.at(0) == "Enemy"){//  || objectAttribs.at(0) == "Enemy") { // platform vs ...
				int ID = stoi(objectAttribs.at(1));

				// check if there is already a platform with this ID
				FixedPathEntity *ent;
				if (entities.count(ID) == 1){
					ent = entities[ID]; // exists, add new data to it
				} else {
					ent = new FixedPathEntity(); // else create new and store
					ent->setID(ID);
					entities[ID] = ent;
				}
				if (objectAttribs.at(2) == "spawn"){ // spawn vs path
					int tileID = stoi(object->Attribute("gid"));
					if (objectAttribs.at(0) == "Enemy") {
						ent->setEnemy();
					}
					ent->setTileID(tileID); // tile idx in spritesheet
					// position is bottom left => correct to top left
					// see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object
					ent->setSpawn(glm::vec2(xPos, yPos - height));
					ent->setSize(glm::vec2(width, height));
					// add texture coordinates of tile
					// TODO extract: duplicate of this code in prepareArrays()
					glm::vec2 textureCoords = getTextureCoordsForTileID(tileID);
					ent->setTextureBounds(textureCoords, getCorrectedTileTextureSize());
					// add bounding shape to platform
					if (tileTypeByID.count(tileID - 1) == 1){ // -1 because IDs start with 1
						// custom collision bounds (rescaled to fit multi-tile)
						BoundingShape * tileBounds = tileTypeByID[tileID - 1]->collisionBounds;
						BoundingShape * copyTileBounds = tileBounds->clone();
						copyTileBounds->rescale(ent->getSize() / float(tileSize));
						ent->setBoundingShape(copyTileBounds);
					}
				} else if (objectAttribs.at(2) == "path"){
					// path of platform
					ent->setPathStart(glm::vec2(xPos, yPos));
					ent->setPathEnd(glm::vec2(xPos+width, yPos+height));
				}
			}
			object = object->NextSiblingElement("object");
		}
	}

	return true;
}

glm::vec2 TileMap::getHalfTexel(){
	return glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
}

glm::vec2 TileMap::getTextureCoordsForTileID(int tileID){
	return glm::vec2(float((tileID-1) % tilesheetSize.x) / tilesheetSize.x, float((tileID-1) / tilesheetSize.x) / tilesheetSize.y);
}

glm::vec2 TileMap::getCorrectedTileTextureSize(){ // size - halfTexel
	return this->tileTexSize - getHalfTexel();
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

void TileMap::prepareArrays(ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;

	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(i * tileSize, j * tileSize);
				texCoordTile[0] = getTextureCoordsForTileID(tile);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += getHalfTexel();
				texCoordTile[1] -= getHalfTexel();
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

	x = min(pos.x / tileSize, mapSize.x-1);
	y0 = min(pos.y / tileSize, mapSize.y-1);
	y1 = min((pos.y + size.y - 1) / tileSize, mapSize.y-1);
	for(int y=y0; y<=y1; y++)
	{
		if (!(std::find(std::begin(non_collision_tiles), std::end(non_collision_tiles), map[y*mapSize.x + x]) != std::end(non_collision_tiles)))
		{
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = min((pos.x + size.x - 1) / tileSize, mapSize.x-1);
	y0 = min(pos.y / tileSize, mapSize.y-1);
	y1 = min((pos.y + size.y - 1) / tileSize, mapSize.y-1);
	for(int y=y0; y<=y1; y++)
	{
		if (!(std::find(std::begin(non_collision_tiles), std::end(non_collision_tiles), map[y*mapSize.x + x]) != std::end(non_collision_tiles)))
		{
			return true;
		}
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
		if (!(std::find(std::begin(non_collision_tiles), std::end(non_collision_tiles), map[y*mapSize.x + x]) != std::end(non_collision_tiles)))
		{
			*posY = tileSize * y - size.y;
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		int aux = map[y*mapSize.x + x];
		if (!(std::find(std::begin(non_collision_tiles), std::end(non_collision_tiles), map[y*mapSize.x + x]) != std::end(non_collision_tiles)))
		{
			*posY = tileSize * (y + 1);
			return true;
		}
	}
	return false;
}

bool TileMap::triggerDeath(const glm::ivec2 &pos, const glm::ivec2 &size, bool upsidedown) const {
	int x0, y0, x1, y1, y0disp, y1disp;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	if (upsidedown) {
		y0disp = 0;
		y1disp = -7;
	}
	else {
		y0disp = 7;
		y1disp = 0;
	}
	y0 = (pos.y - 1 + y0disp) / tileSize;
	y1 = (pos.y + size.y - 1 + y1disp) / tileSize;
	for (int x = x0; x <= x1; x++) {
		for (int y = y0; y <= y1; y++) {
			if ((std::find(std::begin(death_tiles), std::end(death_tiles), map[y*mapSize.x + x]) != std::end(death_tiles))) {
				return true;
			}
		}
	}
	return false;
}
