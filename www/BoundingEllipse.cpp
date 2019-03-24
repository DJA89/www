#include "BoundingEllipse.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "Intersection.h"

using namespace std;

bool BoundingEllipse::intersects(BoundingShape & shape){
	shape.intersects(*this);
}

bool BoundingEllipse::intersects(BoundingEllipse & ellipse){
	// ellipse with ellipse
	return Intersection::check(*this, ellipse);
}

bool BoundingEllipse::intersects(AxisAlignedBoundingBox & box){
	// ellipse with aabb
	return Intersection::check(*this, box);
}
