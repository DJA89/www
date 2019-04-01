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
	glm::vec2 center = be.getCenter();
	float radius = be.getRadius();
	glm::vec2 pos = aabb.getPosition();
	glm::vec2 size = aabb.getSize();
	if (center.x <= pos.x && center.y <= pos.y) { //Arriba a la izquierda
		float dist = glm::distance(center, pos);
		return dist < radius;
	}
	else if (center.x > pos.x && center.x < pos.x + size.x && center.y <= pos.y) { //Arriba
		return center.y + radius > pos.y;
	}
	else if (center.x >= pos.x + size.x && center.y <= pos.y) { //Arriba a la derecha
		float dist = glm::distance(center, glm::vec2(pos.x + size.x, pos.y));
		return dist < radius;
	}
	else if (center.x > pos.x + size.x && center.y > pos.y && center.y < pos.y + size.y) { //A la derecha
		return center.x - radius < pos.x + size.x;
	}
	else if (center.x >= pos.x + size.x && center.y >= pos.y + size.y) { //Abajo a la derecha
		float dist = glm::distance(center, glm::vec2(pos.x + size.x, pos.y + size.y));
		return dist < radius;
	}
	else if (center.x > pos.x && center.x < pos.x + size.x && center.y > pos.y + size.y) { //Abajo
		return center.y - radius > pos.y + size.y;
	}
	else if (center.x <= pos.x && center.y >= pos.y + size.y) { //Abajo a la izquierda
		float dist = glm::distance(center, glm::vec2(pos.x, pos.y + size.y));
		return dist < radius;
	}
	else if (center.x < pos.x && center.y > pos.y && center.y < pos.y + size.y) { //A la izquierda
		return center.x + radius > pos.x;
	}
	return false;
}

bool Intersection::check(const BoundingEllipse & be, const AxisAlignedBoundingBox & aabb){
	return check(aabb, be);
}

bool Intersection::check(const BoundingEllipse & be1, const BoundingEllipse & be2){
	// actual code
	float dist = glm::distance(be1.getPosition() + be1.getCenter(), be2.getPosition() + be2.getCenter());
	return dist < be1.getRadius() + be2.getRadius();
}
