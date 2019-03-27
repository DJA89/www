#ifndef _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE
#define _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE


#include <glm/glm.hpp>
#include "BoundingShape.h"

class AxisAlignedBoundingBox: public BoundingShape
{

public:
	AxisAlignedBoundingBox(glm::vec2 positionInTile, glm::vec2 size) : BoundingShape(positionInTile, size) {};
	bool intersects(const BoundingShape & shape) const;
	bool intersects(const BoundingEllipse & ellipse) const;
	bool intersects(const AxisAlignedBoundingBox & box) const;

private:
};


#endif // _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE
