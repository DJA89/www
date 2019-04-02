#include "BoundingShape.h"
#include "BoundingEllipse.h"
#include "AxisAlignedBoundingBox.h"

void BoundingShape::rescale(glm::vec2 scale){
	this->size *= scale;
	this->positionInTile *= scale;
}

glm::vec2 BoundingShape::getPosition() const {
	return position;
}

glm::vec2 BoundingShape::getPositionInTile() const {
	return positionInTile;
}

void BoundingShape::recalculateFromEntityPosition(glm::vec2 newPosition) {
	position = newPosition + positionInTile;
}

glm::vec2 BoundingShape::getSize() const {
	return size;
}

void BoundingShape::setSize(glm::vec2 newSize){
	size = newSize;
}
