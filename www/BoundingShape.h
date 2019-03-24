#ifndef _BOUNDING_SHAPE_INCLUDE
#define _BOUNDING_SHAPE_INCLUDE


#include <glm/glm.hpp>
class BoundingEllipse;
class AxisAlignedBoundingBox;

class BoundingShape
{

public:
	BoundingShape(glm::vec2 position, glm::vec2 size) :
		position(position), size(size) {};
	virtual bool intersects(const BoundingShape & shape) const = 0;
	virtual bool intersects(const BoundingEllipse & ellipse) const = 0;
	virtual bool intersects(const AxisAlignedBoundingBox & box) const = 0;
	glm::vec2 getPosition(){ return position; }
	glm::vec2 getSize(){ return size; }

protected:
	glm::vec2 position; // top left
	glm::vec2 size; // for ellipse: containing rectangle
};


#endif // _BOUNDING_SHAPE_INCLUDE
