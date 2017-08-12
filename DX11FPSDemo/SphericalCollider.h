#pragma once
#include "Collider.h"

class CollisionDetection;

class SphereCollider :public Collider
{
protected:
	float radius;
	friend class CollisionDetection;
public:
	SphereCollider(float r);
	void setRadius(float r) { radius = r; }
};