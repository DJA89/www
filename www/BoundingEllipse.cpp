#include "BoundingEllipse.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "Intersection.h"

using namespace std;

bool BoundingEllipse::intersects(const BoundingShape & shape) const {
	shape.intersects(*this);
}

bool BoundingEllipse::intersects(const BoundingEllipse & ellipse) const {
	// ellipse with ellipse
	return Intersection::check(*this, ellipse);
}

bool BoundingEllipse::intersects(const AxisAlignedBoundingBox & box) const {
	// ellipse with aabb
	return Intersection::check(*this, box);
}
