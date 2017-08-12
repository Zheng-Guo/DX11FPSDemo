#include "CylindricalCollider.h"

CylindricalCollider::CylindricalCollider(float h, float r) :Collider(ColliderType::Cylindrical),
halfHeight(h),
radius(r)
{
}
