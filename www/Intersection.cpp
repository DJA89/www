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
	// aabb with aabb collision detection
	glm::vec2 pos1 = aabb1.getPosition();
	glm::vec2 size1 = aabb1.getSize();
	glm::vec2 pos2 = aabb2.getPosition();
	glm::vec2 size2 = aabb2.getSize();
	return (pos1.x < pos2.x + size2.x &&
					pos1.x + size1.x > pos2.x &&
					pos1.y < pos2.y + size2.y &&
					pos1.y + size1.y > pos2.y);
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

glm::vec2 Intersection::getMTV(const BoundingShape & bs1, const BoundingShape & bs2){
	return bs1.getMTV(bs2);
}

glm::vec2 Intersection::getMTV(const AxisAlignedBoundingBox & aabb1, const AxisAlignedBoundingBox & aabb2){
		glm::vec2 pos1 = aabb1.getPosition();
		glm::vec2 size1 = aabb1.getSize();
		glm::vec2 pos2 = aabb2.getPosition();
		glm::vec2 size2 = aabb2.getSize();
		int overlapX = 0;
		int overlapY = 0;
		if (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x){
			// collision in x-direction
			overlapX = min(abs(pos1.x - pos2.x + size2.x), abs(pos1.x + size1.x - pos2.x));
		}
		if (pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y){
			// collision in y-direction
			overlapY = min(abs(pos1.y - pos2.y + size2.y), abs(pos1.y + size1.y - pos2.y));
		}
		return glm::vec2(overlapX, overlapY);
}

glm::vec2 Intersection::getMTV(const AxisAlignedBoundingBox & aabb, const BoundingEllipse & be){
	cout << "NOT IMPLEMENTED: aabb-ellipse" << endl;
	return glm::vec2(0, 0);
}

glm::vec2 Intersection::getMTV(const BoundingEllipse & be, const AxisAlignedBoundingBox & aabb){
	return getMTV(aabb, be);
}

glm::vec2 Intersection::getMTV(const BoundingEllipse & be1, const BoundingEllipse & be2){
	cout << "NOT IMPLEMENTED: aabb-ellipse" << endl;
	return glm::vec2(0, 0);
}
