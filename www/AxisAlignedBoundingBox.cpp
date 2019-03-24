#include "AxisAlignedBoundingBox.h"
#include "BoundingShape.h"
#include "BoundingEllipse.h"
#include "Intersection.h"

using namespace std;

bool AxisAlignedBoundingBox::intersects(BoundingShape & shape){
	shape.intersects(*this);
}

bool AxisAlignedBoundingBox::intersects(BoundingEllipse & ellipse){
	// aabb with ellipse
	return Intersection::check(*this, ellipse);
}

bool AxisAlignedBoundingBox::intersects(AxisAlignedBoundingBox & box){
	// aabb with aabb
	Intersection::check(*this, box);
}
