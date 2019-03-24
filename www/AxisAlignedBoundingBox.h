#ifndef _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE
#define _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE


#include <glm/glm.hpp>
#include "BoundingShape.h"

class AxisAlignedBoundingBox: public BoundingShape
{

public:
	AxisAlignedBoundingBox(glm::vec2 position, glm::vec2 size) : BoundingShape(position, size) {};
	bool intersects(BoundingShape & shape);
	bool intersects(BoundingEllipse & ellipse);
	bool intersects(AxisAlignedBoundingBox & box);

private:
};


#endif // _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE
