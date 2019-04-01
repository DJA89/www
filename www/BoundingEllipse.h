#ifndef _BOUNDING_ELLIPSE_INCLUDE
#define _BOUNDING_ELLIPSE_INCLUDE


#include <glm/glm.hpp>
#include "BoundingShape.h"

class BoundingEllipse: public BoundingShape
{


private:
	glm::vec2 center;
	float radius;

public:
	BoundingEllipse(glm::vec2 positionInTile, glm::vec2 size) : BoundingShape(positionInTile, size) {
		center = glm::vec2(positionInTile.x + size.x, positionInTile.y + size.y);
		radius = size.x / 2;
	};
	BoundingShape * clone() const { return new BoundingEllipse(*this); }
	bool intersects(const BoundingShape & shape) const;
	bool intersects(const BoundingEllipse & ellipse) const;
	bool intersects(const AxisAlignedBoundingBox & box) const;
	const glm::vec2 getCenter() const { return center; };
	const float getRadius() const { return radius; };

protected:
	BoundingEllipse(const BoundingEllipse & b) : BoundingShape(b) {};

};


#endif // _BOUNDING_ELLIPSE_INCLUDE
