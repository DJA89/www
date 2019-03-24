#ifndef _BOUNDING_SHAPE_INCLUDE
#define _BOUNDING_SHAPE_INCLUDE


#include <glm/glm.hpp>

class BoundingShape
{

public:
	BoundingShape(glm::vec2 position, glm::vec2 size) :
		position(position), size(size) {};

protected:
	glm::vec2 position; // top left
	glm::vec2 size; // for ellipse: containing rectangle
};


#endif // _BOUNDING_SHAPE_INCLUDE
