#ifndef _BOUNDING_SHAPE_INCLUDE
#define _BOUNDING_SHAPE_INCLUDE


#include <glm/glm.hpp>

class BoundingShape
{

public:

protected:
	glm::ivec2 position; // top left
	glm::ivec2 size; // for ellipse: containing rectangle
};


#endif // _BOUNDING_SHAPE_INCLUDE
