#include "Intersection.h"
#include "AxisAlignedBoundingBox.h"
#include "BoundingEllipse.h"

using namespace std;

bool Intersection::check(AxisAlignedBoundingBox aabb1, AxisAlignedBoundingBox aabb2){
	// actual code
	return false;
}

bool Intersection::check(AxisAlignedBoundingBox aabb, BoundingEllipse be){
	// actual code
	return false;
}

bool Intersection::check(BoundingEllipse be, AxisAlignedBoundingBox aabb){
	return check(aabb, be);
}

bool Intersection::check(BoundingEllipse be1, BoundingEllipse be2){
	// actual code
	return false;
}
