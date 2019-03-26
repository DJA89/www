#include "AxisAlignedBoundingBox.h"
#include "BoundingShape.h"
#include "BoundingEllipse.h"
#include "Intersection.h"

using namespace std;

bool AxisAlignedBoundingBox::intersects(const BoundingShape & shape) const {
	shape.intersects(*this);
}

bool AxisAlignedBoundingBox::intersects(const BoundingEllipse & ellipse) const {
	// aabb with ellipse
	return Intersection::check(*this, ellipse);
}

bool AxisAlignedBoundingBox::intersects(const AxisAlignedBoundingBox & box) const {
	// aabb with aabb
	Intersection::check(*this, box);
}
