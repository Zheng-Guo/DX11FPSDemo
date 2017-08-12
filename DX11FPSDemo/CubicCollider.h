#pragma once
#include "Collider.h"

class CollisionDetection;

class CubicCollider :public Collider
{
protected:
	XMFLOAT3 corner1, corner2;
	float halfWidth, halfHeight, halfThickness;
	friend class CollisionDetection;
public:
	CubicCollider();
	void setHalfWidth(float w) { halfWidth = w; }
	void setHalfHeight(float h) { halfHeight = h; }
	void setHalfThickness(float t) { halfThickness = t; }
};