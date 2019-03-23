#ifndef _BOUNDING_SHAPE_INCLUDE
#define _BOUNDING_SHAPE_INCLUDE


#include <glm/glm.hpp>

class BoundingShape
{

public:
	BoundingShape(glm::vec2 position, glm::vec2 size) :
		position(position), size(size) {};
	// inline void setPosition(glm::vec2 pos){ this->position = pos; };
	// inline void setSize(glm::vec2 size){ this->size = size; };

protected:
	glm::vec2 position; // top left
	glm::vec2 size; // for ellipse: containing rectangle
};


#endif // _BOUNDING_SHAPE_INCLUDE
