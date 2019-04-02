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
		// see: https://developer.roblox.com/articles/2D-Collision-Detection
		if (Intersection::check(aabb1, aabb2)){
			glm::vec2 edge1 = glm::vec2(pos1.x - (pos2.x + size2.x), 0); // left
			glm::vec2 edge2 = glm::vec2((pos1.x + size1.x) - pos2.x, 0); // right
			glm::vec2 edge3 = glm::vec2(0, pos1.y - (pos2.y + size2.y)); // top
			glm::vec2 edge4 = glm::vec2(0, (pos1.y + size1.y) - pos2.y); // bottom
			// I'm tired, ok!
			glm::vec2 shortestEdge = edge1;
			if (glm::length(edge2) < glm::length(shortestEdge)){
				shortestEdge = edge2;
			}
			if (glm::length(edge3) < glm::length(shortestEdge)){
				shortestEdge = edge3;
			}
			if (glm::length(edge4) < glm::length(shortestEdge)){
				shortestEdge = edge4;
			}
			return shortestEdge;
		} else {
			return glm::vec2(0, 0);
		}
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
