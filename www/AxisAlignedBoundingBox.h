#ifndef _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE
#define _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE


#include <glm/glm.hpp>

class AxisAlignedBoundingBox: public BoundingShape
{

public:
	AxisAlignedBoundingBox(glm::vec2 position, glm::vec2 size) :
		BoundingShape(position, size) {};

private:
};


#endif // _AXIS_ALIGNED_BOUNDING_BOX_INCLUDE
