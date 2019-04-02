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
#include "Intersection.h"

using namespace std;
namespace xml = tinyxml2;

// non collidable tiles (which are not entities)
const int TileMap::non_collision_tiles[1] = {EMPTY_TILE};

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
	// remove all animated tiles
	for (auto it = animatedTiles.begin(); it != animatedTiles.cend(); ++it ){
		delete it->second;
	}
	animatedTiles.clear();
	// remove all flames
	for (auto it = flames.begin(); it != flames.cend(); ++it ){
		delete *it; // first => because vector
	}
	flames.clear();
	// remove all conveyor belts
	for (auto it = conveyorBelts.begin(); it != conveyorBelts.cend(); ++it ){
		delete *it; // first => because vector
	}
	conveyorBelts.clear();
	for (auto it = cbfs.begin(); it != cbfs.cend(); ++it ){
		delete it->second;
	}
	cbfs.clear();
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
	return TileMap::loadLevelTmx(levelFile);
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

	// get tileID offset used for levelmap (levelMap-ID = spritesheet-ID + offset)
	tileIDOffset = stoi(mapConf->FirstChildElement("tileset")->Attribute("firstgid"));

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

	// load collision bounding-shapes for tiles (tilesheet => no offset)
	const xml::XMLElement * tile;
	// for each tile found
	tile = tileSetConf->FirstChildElement("tile");
	while (tile){
		// extract tile attributes
		int tileID = stoi(tile->Attribute("id"));
		// load custom collision boxes (if it has)
		const xml::XMLElement * objectGroup = tile->FirstChildElement("objectgroup");
		if (objectGroup != NULL) {
			const xml::XMLElement * object = objectGroup->FirstChildElement("object");
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
		}
		// load animations (if it has)
		const xml::XMLElement * animation = tile->FirstChildElement("animation");
		if (animation != NULL){
			const xml::XMLElement * frame = animation->FirstChildElement("frame");
			vector<int> * frames = new vector<int>();
			while(frame){
				int frameTileID = stoi(frame->Attribute("tileid"));
				frames->push_back(frameTileID);
				// next frame
				frame = frame->NextSiblingElement("frame");
			}
			animatedTiles[tileID] = frames;
		}
		// next iteration
		tile = tile->NextSiblingElement("tile");
	}
	tile = NULL; // not needed anymore

	// extract tilemap data (levelmap => offset)
	istringstream tiles (mapConf->FirstChildElement("layer")->FirstChildElement("data")->GetText());
	map = new int[mapSize.x * mapSize.y];
	string row;
	getline(tiles, row); // skip empty line
	for (int j = 0; j < mapSize.y; j++){
		getline(tiles, row); // for each row
		istringstream ss(row);
		for (int i = 0; i < mapSize.x; i++){
			string tileID_s;
			getline(ss, tileID_s, ','); // split into tiles
			if (isNumber(tileID_s)){
				// INFO: 0 means empty, texture tiles start with 1
				int tileID = stoi(tileID_s) - tileIDOffset; // remove offset
				if (tileID <= 0){ // tile has tileid=0 or is empty
					map[j*mapSize.x + i] = 0; // empty tiles => 0
				} else if (animatedTiles.count(tileID) == 0){ // not animated
					// add to map
					map[j*mapSize.x + i] = tileID;
				} else { // is animated
					map[j*mapSize.x + i] = 0; // empty
					// create new Entity (for each *single* tile)
					bool isFireTile = (tileID == FIRE_FLOOR || tileID == FIRE_CEILING || tileID == FIRE_RIGHT || tileID == FIRE_LEFT);
					bool isConveyorBeltTile = (tileID == WATER_FLOOR_RIGHT || tileID == WATER_CEILING_RIGHT || tileID == WATER_FLOOR_LEFT || tileID == WATER_CEILING_LEFT);
					Entity * newTile;
					if (isFireTile){
						newTile = new DeathTile();
					} else { // if (isConveyorBeltTile)
						newTile = new ConveyorBelt();
					}
					newTile->setTileID(tileID);
					newTile->setPosition(glm::vec2(i*tileSize, j*tileSize));
					newTile->setSize(glm::vec2(tileSize, tileSize));
					// set texture
					// add texture coordinates of tile
					glm::vec2 textureCoords = getTextureCoordsForTileID(tileID);
					newTile->setTextureBounds(textureCoords, getCorrectedTileTextureSize());
					// add bounding shape
					if (tileTypeByID.count(tileID) == 1){ // if has custom collision bounds
						// custom collision bounds (rescaled to fit multi-tile)
						BoundingShape * tileBounds = tileTypeByID[tileID]->collisionBounds;
						BoundingShape * copyTileBounds = tileBounds->clone();
						newTile->setBoundingShape(copyTileBounds);
					}
					// store
					if (isFireTile) {
						flames.push_back((DeathTile *)newTile); // store
					} else { // if (isConveyorBeltTile)
						conveyorBelts.push_back((ConveyorBelt *)newTile); // store
					}
				}
			}
		}
	}

	// extract entities in object layer (levelmap => offset)
	const xml::XMLElement* objectgroup = mapConf->FirstChildElement("objectgroup");
	if (objectgroup) {
		const xml::XMLElement* object;
		// for each object found
		object = objectgroup->FirstChildElement("object");
		while (object) {
			string objectName = object->Attribute("name");
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

			vector<string> objectAttribs = Utils::split(objectName, '_');
			if (objectAttribs.at(0) == "Checkpoint"){
				Checkpoint * newCheckPoint = new Checkpoint();

				int ID = stoi(object->Attribute("id"));
				int tileID = stoi(object->Attribute("gid")) - tileIDOffset; // remove offset
				newCheckPoint->setTileID(tileID); // tile idx in spritesheet
				// position is bottom left => correct to top left
				// see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object
				newCheckPoint->setPosition(glm::vec2(xPos, yPos - height));
				newCheckPoint->setSize(glm::vec2(width, height));
				// add texture coordinates of tile
				glm::vec2 textureCoords = getTextureCoordsForTileID(tileID);
				newCheckPoint->setTextureBounds(textureCoords, getCorrectedTileTextureSize());
				// add bounding shape to platform
				if (tileTypeByID.count(tileID) == 1){
					// custom collision bounds (rescaled to fit multi-tile)
					BoundingShape * tileBounds = tileTypeByID[tileID]->collisionBounds;
					BoundingShape * copyTileBounds = tileBounds->clone();
					copyTileBounds->rescale(newCheckPoint->getSize() / float(tileSize));
					newCheckPoint->setBoundingShape(copyTileBounds);
				}
				checkpoints[ID] = newCheckPoint;
			} else if (objectAttribs.at(0) == "Platform" || objectAttribs.at(0) == "Enemy"){//  || objectAttribs.at(0) == "Enemy") { // platform vs ...
				int ID = stoi(objectAttribs.at(1));

				// check if there is already a platform with this ID
				FixedPathEntity *ent;
				if (entities.count(ID) == 1) {
					ent = dynamic_cast<FixedPathEntity*>(entities[ID]); // exists, add new data to it
				}
				else {
					ent = new FixedPathEntity(); // else create new and store
					ent->setID(ID);
					entities[ID] = ent;
				}
				if (objectAttribs.at(2) == "spawn"){ // spawn vs path
					int tileID = stoi(object->Attribute("gid")) - tileIDOffset; // remove offset
					if (objectAttribs.at(0) == "Enemy") {
						ent->setEnemy(true);
					}
					else {
						ent->setEnemy(false);
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
					if (tileTypeByID.count(tileID) == 1){
						// custom collision bounds (rescaled to fit multi-tile)
						BoundingShape * tileBounds = tileTypeByID[tileID]->collisionBounds;
						BoundingShape * copyTileBounds = tileBounds->clone();
						copyTileBounds->rescale(ent->getSize() / float(tileSize));
						ent->setBoundingShape(copyTileBounds);
					}
				}
				else if (objectAttribs.at(2) == "path") {
					// path of platform
					ent->setPathStart(glm::vec2(xPos, yPos));
					ent->setPathEnd(glm::vec2(xPos + width, yPos + height));
				}
			}
			else if (objectAttribs.at(0) == "cb") {
				int ID = stoi(objectAttribs.at(1));
				ConveyorBelt *cb;
				if (entities.count(ID) == 1) {
					cb = dynamic_cast<ConveyorBelt*>(entities[ID]); // exists, add new data to it
				}
				else {
					cb = new ConveyorBelt(); // else create new and store
					cb->setID(ID);
					entities[ID] = cb;
				}
				int tileID = stoi(object->Attribute("gid"));
				cb->setTileID(tileID); // tile idx in spritesheet
				// position is bottom left => correct to top left
				// see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object
				cb->setPosition(glm::vec2(xPos, yPos - height));
				cb->setSize(glm::vec2(width, height));
				// add texture coordinates of tile
				// TODO extract: duplicate of this code in prepareArrays()
				glm::vec2 halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
				glm::vec2 textureCoords = glm::vec2(float((tileID - 1) % tilesheetSize.x) / tilesheetSize.x, float((tileID - 1) / tilesheetSize.x) / tilesheetSize.y);
				cb->setTextureBounds(textureCoords, tileTexSize - halfTexel);
				char aboveDir = objectAttribs.at(2)[0];
				if (aboveDir == 'r') {
					cb->setAboveVelocity(2);
				}
				else if (aboveDir == 'l') {
					cb->setAboveVelocity(-2);
				}
				else {
					cb->setAboveVelocity(0);
				}
				char belowDir = objectAttribs.at(2)[1];
				if (belowDir == 'r') {
					cb->setBelowVelocity(2);
				}
				else if (belowDir == 'l') {
					cb->setBelowVelocity(-2);
				}
				else {
					cb->setBelowVelocity(0);
				}
				if (tileTypeByID.count(tileID - 1) == 1) { // -1 because IDs start with 1
					// custom collision bounds (rescaled to fit multi-tile)
					BoundingShape * tileBounds = tileTypeByID[tileID - 1]->collisionBounds;
					BoundingShape * copyTileBounds = tileBounds->clone();
					copyTileBounds->rescale(cb->getSize() / float(tileSize));
					cb->setBoundingShape(copyTileBounds);
				}
			}
			if (objectAttribs.at(0) == "cbf") {
				int ID = stoi(objectAttribs.at(1));

				// check if there is already a platform with this ID
				ConveyorBelt *cbf;
				if (cbfs.count(ID) == 1) {
					cbf = cbfs[ID]; // exists, add new data to it
				}
				else {
					cbf = new ConveyorBelt(); // else create new and store
					cbf->setID(ID);
					cbfs[ID] = cbf;
				}
				int tileID = stoi(object->Attribute("gid"));
				cbf->setTileID(tileID); // tile idx in spritesheet
				// position is bottom left => correct to top left
				// see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object
				cbf->setPosition(glm::vec2(xPos, yPos - height));
				cbf->setSize(glm::vec2(width, height));
				// add texture coordinates of tile
				// TODO extract: duplicate of this code in prepareArrays()
				glm::vec2 halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
				glm::vec2 textureCoords = glm::vec2(float((tileID - 1) % tilesheetSize.x) / tilesheetSize.x, float((tileID - 1) / tilesheetSize.x) / tilesheetSize.y);
				cbf->setTextureBounds(textureCoords, tileTexSize - halfTexel);
				// add bounding shape to platform
				if (tileTypeByID.count(tileID - 1) == 1) { // -1 because IDs start with 1
					// custom collision bounds (rescaled to fit multi-tile)
					BoundingShape * tileBounds = tileTypeByID[tileID - 1]->collisionBounds;
					BoundingShape * copyTileBounds = tileBounds->clone();
					copyTileBounds->rescale(cbf->getSize() / float(tileSize));
					cbf->setBoundingShape(copyTileBounds);
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
	return glm::vec2(float(tileID % tilesheetSize.x) / tilesheetSize.x, float(tileID / tilesheetSize.x) / tilesheetSize.y);
}

glm::vec2 TileMap::getCorrectedTileTextureSize(){ // size - halfTexel
	return this->tileTexSize - getHalfTexel();
}

bool TileMap::isNumber(const string &toCheck){
	return !toCheck.empty() && toCheck.find_first_not_of("-0123456789") == std::string::npos;
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

bool TileMap::tileIsCollidable(int tileID) const {
	// if find runs to end (doesnt find) in non_collision_tiles => collidable
	return (std::find(std::begin(non_collision_tiles),
	                  std::end(non_collision_tiles),
	                  tileID)
	        == std::end(non_collision_tiles));
}

glm::vec2 TileMap::getMinimumTranslationVector(const glm::ivec2 &playerPos, const glm::ivec2 &playerSize) const {
	// collision shapes
	BoundingShape * playerCollisionBounds = new AxisAlignedBoundingBox(glm::ivec2(0, 0), playerSize);
	playerCollisionBounds->recalculateFromEntityPosition(playerPos);
	BoundingShape * tileCollisionBounds;
	glm::vec2 maximumMTV = glm::vec2(0.f, 0.f);

	int x0, x1, y0, y1;
	x0 = min(playerPos.x / tileSize, mapSize.x-1);
	x1 = min((playerPos.x + playerSize.x - 1) / tileSize, mapSize.x-1);
	y0 = min(playerPos.y / tileSize, mapSize.y-1);
	y1 = min((playerPos.y + playerSize.y - 1) / tileSize, mapSize.y-1);
	for (int y = y0; y <= y1; y++){
		for (int x = x0; x <= x1; x++){
			// TODO: quite inefficient, better only on tiles touched by player...
			if (tileIsCollidable(map[y*mapSize.x + x])){
				// if collidable
				tileCollisionBounds = new AxisAlignedBoundingBox(
				                        glm::ivec2(0, 0),
				                        glm::ivec2(tileSize, tileSize));
				tileCollisionBounds->recalculateFromEntityPosition(glm::ivec2(x * tileSize, y * tileSize));
				glm::vec2 currentMTV = Intersection::getMTV(*tileCollisionBounds, *playerCollisionBounds);
				if (abs(currentMTV.x) > abs(maximumMTV.x)){
					maximumMTV.x = currentMTV.x;
				}
				if (abs(currentMTV.y) > abs(maximumMTV.y)){
					maximumMTV.y = currentMTV.y;
				}
				delete tileCollisionBounds;
			}
		}
	}
	return maximumMTV;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = min(pos.x / tileSize, mapSize.x-1);
	y0 = min(pos.y / tileSize, mapSize.y-1);
	y1 = min((pos.y + size.y - 1) / tileSize, mapSize.y-1);
	for(int y=y0; y<=y1; y++)
	{
		if (tileIsCollidable(map[y*mapSize.x + x])){
			return true;
		}
	}
	// no collision with collidable tile
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
		if (tileIsCollidable(map[y*mapSize.x + x])){
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
		if (tileIsCollidable(map[y*mapSize.x + x])){
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
		if (tileIsCollidable(map[y*mapSize.x + x])){
			*posY = tileSize * (y + 1);
			return true;
		}
	}
	return false;
}
