#include "Intersection.h"
#include "BoundingShape.h"
#include "AxisAlignedBoundingBox.h"
#include "BoundingEllipse.h"
#include <iostream>

using namespace std;

bool check(BoundingShape & bs1, BoundingShape & bs2){
	return bs1.intersects(bs2);
}

bool Intersection::check(AxisAlignedBoundingBox & aabb1, AxisAlignedBoundingBox & aabb2){
	// actual code
	cout << "NOT IMPLEMENTED" << endl;
	return false;
}

bool Intersection::check(AxisAlignedBoundingBox & aabb, BoundingEllipse & be){
	// actual code
	cout << "NOT IMPLEMENTED" << endl;
	return false;
}

bool Intersection::check(BoundingEllipse & be, AxisAlignedBoundingBox & aabb){
	return check(aabb, be);
}

bool Intersection::check(BoundingEllipse & be1, BoundingEllipse & be2){
	// actual code
	cout << "NOT IMPLEMENTED" << endl;
	return false;
}
