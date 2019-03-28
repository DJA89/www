#ifndef _BOUNDING_ELLIPSE_INCLUDE
#define _BOUNDING_ELLIPSE_INCLUDE


#include <glm/glm.hpp>
#include "BoundingShape.h"

class BoundingEllipse: public BoundingShape
{

public:
	BoundingEllipse(glm::vec2 positionInTile, glm::vec2 size) : BoundingShape(positionInTile, size) {};
	BoundingShape * clone() const { return new BoundingEllipse(*this); }
	bool intersects(const BoundingShape & shape) const;
	bool intersects(const BoundingEllipse & ellipse) const;
	bool intersects(const AxisAlignedBoundingBox & box) const;

protected:
	BoundingEllipse(const BoundingEllipse & b) : BoundingShape(b) {};

private:
};


#endif // _BOUNDING_ELLIPSE_INCLUDE
