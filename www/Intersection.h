#ifndef _INTERSECTION_INCLUDE
#define _INTERSECTION_INCLUDE


#include <glm/glm.hpp>

class AxisAlignedBoundingBox;
class BoundingEllipse;
class BoundingShape;

class Intersection
{

public:
	static bool check(const BoundingShape & bs1, const BoundingShape & bs2);
	static bool check(const AxisAlignedBoundingBox & aabb1, const AxisAlignedBoundingBox & aabb2);
	static bool check(const AxisAlignedBoundingBox & aabb, const BoundingEllipse & be);
	static bool check(const BoundingEllipse & be, const AxisAlignedBoundingBox & aabb);
	static bool check(const BoundingEllipse & be1, const BoundingEllipse & be2);
	static bool check(const glm::vec2 & ps1, const glm::vec2 & ps2);

	static glm::vec2 getMTV(const BoundingShape & bs1, const BoundingShape & bs2);
	static glm::vec2 getMTV(const AxisAlignedBoundingBox & aabb1, const AxisAlignedBoundingBox & aabb2);
	static glm::vec2 getMTV(const AxisAlignedBoundingBox & aabb, const BoundingEllipse & be);
	static glm::vec2 getMTV(const BoundingEllipse & be, const AxisAlignedBoundingBox & aabb);
	static glm::vec2 getMTV(const BoundingEllipse & be1, const BoundingEllipse & be2);

private:
};


#endif // _INTERSECTION_INCLUDE
