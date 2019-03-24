#ifndef _BOUNDING_ELLIPSE_INCLUDE
#define _BOUNDING_ELLIPSE_INCLUDE


#include <glm/glm.hpp>
#include "BoundingShape.h"

class BoundingEllipse: public BoundingShape
{

public:
	BoundingEllipse(glm::vec2 position, glm::vec2 size) : BoundingShape(position, size) {};
	bool intersects(BoundingShape & shape);
	bool intersects(BoundingEllipse & ellipse);
	bool intersects(AxisAlignedBoundingBox & box);

private:
};


#endif // _BOUNDING_ELLIPSE_INCLUDE
