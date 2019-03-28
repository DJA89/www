#ifndef _BOUNDING_SHAPE_INCLUDE
#define _BOUNDING_SHAPE_INCLUDE


#include <glm/glm.hpp>
class BoundingEllipse;
class AxisAlignedBoundingBox;

class BoundingShape
{

public:
	BoundingShape(glm::vec2 positionInTile, glm::vec2 size) : positionInTile(positionInTile), size(size) {};
	virtual BoundingShape * clone() const = 0;
	void rescale(glm::vec2 scale){ size *= scale; }
	virtual bool intersects(const BoundingShape & shape) const = 0;
	virtual bool intersects(const BoundingEllipse & ellipse) const = 0;
	virtual bool intersects(const AxisAlignedBoundingBox & box) const = 0;
	glm::vec2 getPosition() const { return position; }
	void recalculateFromEntityPosition(glm::vec2 newPosition) { position = newPosition + positionInTile; }
	glm::vec2 getSize() const { return size; }

protected:
	// copy constructor
	BoundingShape(const BoundingShape & b) : positionInTile(b.positionInTile), position(b.position), size(b.size) {};

protected:
	glm::vec2 positionInTile; // from tile top left
	glm::vec2 position = glm::vec2(0, 0); // top left
	glm::vec2 size; // for ellipse: containing rectangle
};


#endif // _BOUNDING_SHAPE_INCLUDE
