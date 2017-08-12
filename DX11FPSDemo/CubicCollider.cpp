#include "CubicCollider.h"

CubicCollider::CubicCollider() :Collider(ColliderType::Cubic),
corner1(-1.f, -1.f, -1.f),
corner2(1.f, 1.f, 1.f),
halfWidth(1.f),
halfHeight(1.f),
halfThickness(1.f)
{
}
