#include "BoundingEllipse.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "Intersection.h"

using namespace std;

bool BoundingEllipse::intersects(const BoundingShape & shape) const {
	return shape.intersects(*this);
}

bool BoundingEllipse::intersects(const BoundingEllipse & ellipse) const {
	// ellipse with ellipse
	return Intersection::check(*this, ellipse);
}

bool BoundingEllipse::intersects(const AxisAlignedBoundingBox & box) const {
	// ellipse with aabb
	return Intersection::check(*this, box);
}

glm::vec2 BoundingEllipse::getMTV(const BoundingShape & shape) const {
	return shape.getMTV(*this);
}

glm::vec2 BoundingEllipse::getMTV(const BoundingEllipse & ellipse) const {
	return Intersection::getMTV(*this, ellipse);
}

glm::vec2 BoundingEllipse::getMTV(const AxisAlignedBoundingBox & box) const {
	return Intersection::getMTV(*this, box);
}
