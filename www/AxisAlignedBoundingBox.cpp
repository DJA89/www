#include "AxisAlignedBoundingBox.h"
#include "BoundingShape.h"
#include "BoundingEllipse.h"
#include "Intersection.h"

using namespace std;

bool AxisAlignedBoundingBox::intersects(const BoundingShape & shape) const {
	return shape.intersects(*this);
}

bool AxisAlignedBoundingBox::intersects(const BoundingEllipse & ellipse) const {
	// aabb with ellipse
	return Intersection::check(*this, ellipse);
}

bool AxisAlignedBoundingBox::intersects(const AxisAlignedBoundingBox & box) const {
	// aabb with aabb
	return Intersection::check(*this, box);
}

glm::vec2 AxisAlignedBoundingBox::getMTV(const BoundingShape & shape) const {
	return shape.getMTV(*this);
}

glm::vec2 AxisAlignedBoundingBox::getMTV(const BoundingEllipse & ellipse) const {
	return Intersection::getMTV(*this, ellipse);
}

glm::vec2 AxisAlignedBoundingBox::getMTV(const AxisAlignedBoundingBox & box) const {
	return Intersection::getMTV(*this, box);
}
