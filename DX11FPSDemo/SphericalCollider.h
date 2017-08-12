#pragma once
#include "Collider.h"

class CollisionDetection;

class SphericalCollider :public Collider
{
protected:
	float radius;
	friend class CollisionDetection;
public:
	SphericalCollider(float r);
	void setRadius(float r) { radius = r; }
};