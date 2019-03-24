#ifndef _INTERSECTION_INCLUDE
#define _INTERSECTION_INCLUDE


#include <glm/glm.hpp>

class AxisAlignedBoundingBox;
class BoundingEllipse;
class BoundingShape;

class Intersection
{

public:
	static bool check(BoundingShape & bs1, BoundingShape & bs2);
	static bool check(AxisAlignedBoundingBox & aabb1, AxisAlignedBoundingBox & aabb2);
	static bool check(AxisAlignedBoundingBox & aabb, BoundingEllipse & be);
	static bool check(BoundingEllipse & be, AxisAlignedBoundingBox & aabb);
	static bool check(BoundingEllipse & be1, BoundingEllipse & be2);

private:
};


#endif // _INTERSECTION_INCLUDE
