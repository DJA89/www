#ifndef _BOUNDING_ELLIPSE_INCLUDE
#define _BOUNDING_ELLIPSE_INCLUDE


#include <glm/glm.hpp>

class BoundingEllipse: public BoundingShape
{

public:
	BoundingEllipse(glm::vec2 position, glm::vec2 size) :
		BoundingShape(position, size) {};

private:
};


#endif // _BOUNDING_ELLIPSE_INCLUDE
