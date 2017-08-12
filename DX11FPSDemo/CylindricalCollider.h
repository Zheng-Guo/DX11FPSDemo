#pragma once
#include "Collider.h"

class CollisionDetection;

class CylindricalCollider :public Collider
{
protected:
	float halfHeight;
	float radius;
	friend class CollisionDetection;
public:
	CylindricalCollider(float h = 1.f, float r = 1.f);
	void setHalfHeight(float h) { halfHeight = h; }
	void setRadius(float r) { radius = r; }
};