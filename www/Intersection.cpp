#include "Intersection.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "BoundingEllipse.h"
#include <iostream>

using namespace std;

bool Intersection::check(const BoundingShape & bs1, const BoundingShape & bs2){
	return bs1.intersects(bs2);
}

bool Intersection::check(const AxisAlignedBoundingBox & aabb1, const AxisAlignedBoundingBox & aabb2){
	// actual code
	cout << "NOT IMPLEMENTED: AABB-AABB" << endl;
	return false;
}

bool Intersection::check(const AxisAlignedBoundingBox & aabb, const BoundingEllipse & be){
	// actual code
	cout << "NOT IMPLEMENTED: aabb-ellipse" << endl;
	return false;
}

bool Intersection::check(const BoundingEllipse & be, const AxisAlignedBoundingBox & aabb){
	return check(aabb, be);
}

bool Intersection::check(const BoundingEllipse & be1, const BoundingEllipse & be2){
	// actual code
	cout << "NOT IMPLEMENTED: ellipse-ellipse" << endl;
	return false;
}
