#pragma once
#include "CubicCollider.h"
#include "CylindricalCollider.h"
#include "SphericalCollider.h"

class CollisionDetection
{
public:
	static XMFLOAT3 isOverlappedBetweenCubicAndCylindrical(CubicCollider *c1, CylindricalCollider *c2);
	static XMFLOAT3 isOverlappedBetweenTwoCylindrical(CylindricalCollider *c1, CylindricalCollider *c2);
	static XMFLOAT3 isOverlappedBetweenCubicAndSphere(CubicCollider *c1, SphericalCollider *c2);
	static XMFLOAT3 isOverlappedBetweenCylindricalAndSphere(CylindricalCollider *c1, SphericalCollider *c2);

	static bool isOverlapped(Collider *collider1, Collider *collider2);
};