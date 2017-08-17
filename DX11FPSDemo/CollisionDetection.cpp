#include "CollisionDetection.h"
#include <cmath>

XMFLOAT3 CollisionDetection::isOverlappedBetweenCubicAndCylindrical(CubicCollider *c1, CylindricalCollider *c2)
{
	XMFLOAT3 offset(0.f, 0.f, 0.f);
	XMVECTOR cubeLocalPosition = XMLoadFloat3(&(c1->position));
	XMMATRIX cubeWorldTransform = XMLoadFloat4x4(&(c1->parentTransform));
	XMVECTOR cubeWorldPosition = XMVector3TransformCoord(cubeLocalPosition, cubeWorldTransform);
	XMFLOAT3 cubeCenter;
	XMStoreFloat3(&cubeCenter, cubeWorldPosition);
	XMVECTOR cylinderLocalPosition = XMLoadFloat3(&(c2->position));
	XMMATRIX cylinderWorldTransform = XMLoadFloat4x4(&(c2->parentTransform));
	XMVECTOR cylinderWorldPosition = XMVector3TransformCoord(cylinderLocalPosition, cylinderWorldTransform);
	XMFLOAT3 cylinderCenter;
	XMStoreFloat3(&cylinderCenter, cylinderWorldPosition);
	float centerHorizontalDistance = sqrt((cubeCenter.x - cylinderCenter.x)*(cubeCenter.x - cylinderCenter.x) + (cubeCenter.z - cylinderCenter.z)*(cubeCenter.z - cylinderCenter.z));
	float threshHold = sqrt(c1->halfWidth*c1->halfWidth + c1->halfThickness*c1->halfThickness) + c2->radius;
	if (centerHorizontalDistance >= threshHold)
		return offset;
	float x1 = cubeCenter.x - c1->halfWidth, x2 = cubeCenter.x + c1->halfWidth;
	float y1 = cubeCenter.y - c1->halfHeight, y2 = cubeCenter.y + c1->halfHeight;
	float z1 = cubeCenter.z - c1->halfThickness, z2 = cubeCenter.z + c1->halfThickness;
	float cylinderTop = cylinderCenter.y + c2->halfHeight, cylinderBottom = cylinderCenter.y - c2->halfHeight;
	if (y1 >= cylinderTop || y2 <= cylinderBottom)
		return offset;
	if (cylinderCenter.x>x1&&cylinderCenter.x<x2)
	{
		if (c1->halfThickness + c2->radius > abs(cubeCenter.z - cylinderCenter.z))
		{
			float zOffset = c1->halfThickness + c2->radius - abs(cubeCenter.z - cylinderCenter.z);
			if (c2->velocity.z>0)
			{
				offset = XMFLOAT3(0.f, 0.f, -zOffset);
			}
			else if(c2->velocity.z<0)
			{
				offset = XMFLOAT3(0.f, 0.f, zOffset);
			}
			return offset;
		}
		else return offset;
	}
	if (cylinderCenter.z>z1&&cylinderCenter.z<z2)
	{
		float xOffset = c1->halfWidth + c2->radius - abs(cubeCenter.x - cylinderCenter.x);
		if (c1->halfWidth + c2->radius > abs(cubeCenter.x - cylinderCenter.x))
		{
			if (c2->velocity.x>0)
			{
				offset = XMFLOAT3(-xOffset, 0.f, 0.f);
			}
			else if(c2->velocity.x<0)
			{
				offset = XMFLOAT3(xOffset, 0.f, 0.f);
			}
			return offset;
		}
		else return offset;
	}
	if ((cylinderCenter.x - x1)*(cylinderCenter.x - x1) + (cylinderCenter.z - z1)*(cylinderCenter.z - z1) < (c2->radius*c2->radius))
	{
		if(c2->velocity.x>0&&c2->velocity.z>0)
		{
			float a = (c2->velocity.x*c2->velocity.x) + (c2->velocity.z*c2->velocity.z);
			float b = 2.f*(c2->velocity.x*(x1 - cylinderCenter.x) + c2->velocity.z*(z1 - cylinderCenter.z));
			float c = (x1 - cylinderCenter.x)*(x1 - cylinderCenter.x) + (z1 - cylinderCenter.z)*(z1 - cylinderCenter.z) - c2->radius*c2->radius;
			float determinant = b*b - 4 * a*c;
			if (determinant < 0.f)
				determinant = 0.f;
			float solution1 = (-b + sqrt(determinant)) / 2.f / a, solution2 = (-b - sqrt(determinant)) / 2.f / a;
			float solution;
			if (solution1 <= 0)
				solution = solution2;
			else
				solution = solution1;
			XMVECTOR shift =XMVectorSet(c2->velocity.x, 0.f, c2->velocity.z, 0.f)*-solution;
			XMStoreFloat3(&offset, shift);
		}
		else if(c2->velocity.x>0)
		{
			float shift = c2->radius - abs(x1 - cylinderCenter.x);
			offset = XMFLOAT3(-shift, 0.f, 0.f);
		}
		else if(c2->velocity.z>0)
		{
			float shift = c2->radius - abs(z1 - cylinderCenter.z);
			offset = XMFLOAT3(0.f, 0.f, -shift);
		}
	}
	if ((cylinderCenter.x - x1)*(cylinderCenter.x - x1) + (cylinderCenter.z - z2)*(cylinderCenter.z - z2) < (c2->radius*c2->radius))
	{
		if (c2->velocity.x>0 && c2->velocity.z<0)
		{
			float a = (c2->velocity.x*c2->velocity.x) + (c2->velocity.z*c2->velocity.z);
			float b = 2.f*(c2->velocity.x*(x1 - cylinderCenter.x) + c2->velocity.z*(z2 - cylinderCenter.z));
			float c = (x1 - cylinderCenter.x)*(x1 - cylinderCenter.x) + (z2 - cylinderCenter.z)*(z2 - cylinderCenter.z) - c2->radius*c2->radius;
			float determinant = b*b - 4 * a*c;
			if (determinant < 0.f)
				determinant = 0.f;
			float solution1 = (-b + sqrt(determinant)) / 2.f / a, solution2 = (-b - sqrt(determinant)) / 2.f / a;
			float solution;
			if (solution1 <= 0)
				solution = solution2;
			else
				solution = solution1;
			XMVECTOR shift = XMVectorSet(c2->velocity.x, 0.f, c2->velocity.z, 0.f)*-solution;
			XMStoreFloat3(&offset, shift);
		}
		else if (c2->velocity.x>0)
		{
			float shift = c2->radius - abs(x1 - cylinderCenter.x);
			offset = XMFLOAT3(-shift, 0.f, 0.f);
		}
		else if (c2->velocity.z<0)
		{
			float shift = c2->radius - abs(z2 - cylinderCenter.z);
			offset = XMFLOAT3(0.f, 0.f, shift);
		}
	}
	if ((cylinderCenter.x - x2)*(cylinderCenter.x - x2) + (cylinderCenter.z - z1)*(cylinderCenter.z - z1) < (c2->radius*c2->radius))
	{
		if (c2->velocity.x<0 && c2->velocity.z>0)
		{
			float a = (c2->velocity.x*c2->velocity.x) + (c2->velocity.z*c2->velocity.z);
			float b = 2.f*(c2->velocity.x*(x2 - cylinderCenter.x) + c2->velocity.z*(z1 - cylinderCenter.z));
			float c = (x2 - cylinderCenter.x)*(x2 - cylinderCenter.x) + (z1 - cylinderCenter.z)*(z1 - cylinderCenter.z) - c2->radius*c2->radius;
			float determinant = b*b - 4 * a*c;
			if (determinant < 0.f)
				determinant = 0.f;
			float solution1 = (-b + sqrt(determinant)) / 2.f / a, solution2 = (-b - sqrt(determinant)) / 2.f / a;
			float solution;
			if (solution1 <= 0)
				solution = solution2;
			else
				solution = solution1;
			XMVECTOR shift = XMVectorSet(c2->velocity.x, 0.f, c2->velocity.z, 0.f)*-solution;
			XMStoreFloat3(&offset, shift);
		}
		else if (c2->velocity.x<0)
		{
			float shift = c2->radius - abs(x2 - cylinderCenter.x);
			offset = XMFLOAT3(shift, 0.f, 0.f);
		}
		else if (c2->velocity.z>0)
		{
			float shift = c2->radius - abs(z1 - cylinderCenter.z);
			offset = XMFLOAT3(0.f, 0.f, -shift);
		}
	}
	if ((cylinderCenter.x - x2)*(cylinderCenter.x - x2) + (cylinderCenter.z - z2)*(cylinderCenter.z - z2) < (c2->radius*c2->radius))
	{
		if (c2->velocity.x<0 && c2->velocity.z<0)
		{
			float a = (c2->velocity.x*c2->velocity.x) + (c2->velocity.z*c2->velocity.z);
			float b = 2.f*(c2->velocity.x*(x2 - cylinderCenter.x) + c2->velocity.z*(z2 - cylinderCenter.z));
			float c = (x2 - cylinderCenter.x)*(x2 - cylinderCenter.x) + (z2 - cylinderCenter.z)*(z2 - cylinderCenter.z) - c2->radius*c2->radius;
			float determinant = b*b - 4 * a*c;
			if (determinant < 0.f)
				determinant = 0.f;
			float solution1 = (-b + sqrt(determinant)) / 2.f / a, solution2 = (-b - sqrt(determinant)) / 2.f / a;
			float solution;
			if (solution1 <= 0)
				solution = solution2;
			else
				solution = solution1;
			XMVECTOR shift = XMVectorSet(c2->velocity.x, 0.f, c2->velocity.z, 0.f)*-solution;
			XMStoreFloat3(&offset, shift);
		}
		else if (c2->velocity.x<0)
		{
			float shift = c2->radius - abs(x2 - cylinderCenter.x);
			offset = XMFLOAT3(shift, 0.f, 0.f);
		}
		else if (c2->velocity.z<0)
		{
			float shift = c2->radius - abs(z2 - cylinderCenter.z);
			offset = XMFLOAT3(0.f, 0.f, shift);
		}
	}
	return offset;
}

XMFLOAT3 CollisionDetection::isOverlappedBetweenTwoCylindrical(CylindricalCollider *c1, CylindricalCollider *c2)
{
	XMFLOAT3 offset(0.f, 0.f, 0.f);
	XMVECTOR velocity = XMLoadFloat3(&(c2->velocity)) - XMLoadFloat3(&(c1->velocity));
	if (XMVector3Equal(velocity, XMVectorZero()))
		return offset;
	XMVECTOR c1LocalPosition = XMLoadFloat3(&(c1->position)), c2LocalPosition = XMLoadFloat3(&(c2->position));
	XMMATRIX c1Transform = XMLoadFloat4x4(&(c1->parentTransform)), c2Transform = XMLoadFloat4x4(&(c2->parentTransform));
	XMVECTOR c1Pos = XMVector3TransformCoord(c1LocalPosition, c1Transform), c2Pos = XMVector3TransformCoord(c2LocalPosition, c2Transform);
	XMVECTOR cosTheta = XMVector3Dot(velocity, c1Pos - c2Pos);
	XMVECTOR centerDistance = XMVector3Length(c1Pos - c2Pos);
	XMFLOAT3 ct, cd;
	XMStoreFloat3(&ct, cosTheta);
	XMStoreFloat3(&cd, centerDistance);
	float overlappedDistance = cd.x - c1->radius - c2->radius;
	if (overlappedDistance < 0)
	{
		XMVECTOR v = XMVector3Normalize(velocity)*overlappedDistance / ct.x;
		XMStoreFloat3(&offset, v);
	}
	return offset;
}

XMFLOAT3 CollisionDetection::isOverlappedBetweenCubicAndSphere(CubicCollider * c1, SphericalCollider * c2)
{
	XMFLOAT3 offset(0.f, 0.f, 0.f);
	XMVECTOR cubeLocalPosition = XMLoadFloat3(&(c1->position));
	XMMATRIX cubeWorldTransform = XMLoadFloat4x4(&(c1->parentTransform));
	XMVECTOR cubeWorldPosition = XMVector3TransformCoord(cubeLocalPosition, cubeWorldTransform);
	XMFLOAT3 cubeCenter;
	XMStoreFloat3(&cubeCenter, cubeWorldPosition);
	XMVECTOR sphereLocalPosition = XMLoadFloat3(&(c2->position));
	XMMATRIX sphereWorldTransform = XMLoadFloat4x4(&(c2->parentTransform));
	XMVECTOR sphereWorldPosition = XMVector3TransformCoord(sphereLocalPosition, sphereWorldTransform);
	XMFLOAT3 sphereCenter;
	XMStoreFloat3(&sphereCenter, sphereWorldPosition);
	float centerDistance = XMVectorGetX(cubeWorldPosition - sphereWorldPosition);
	float threshold = sqrt(c1->halfWidth*c1->halfWidth + c1->halfThickness*c1->halfThickness + c1->halfHeight*c1->halfHeight) + c2->radius;
	if (centerDistance >= threshold)
		return offset;
	float x1 = cubeCenter.x - c1->halfWidth, x2 = cubeCenter.x + c1->halfWidth;
	float y1 = cubeCenter.y - c1->halfHeight, y2 = cubeCenter.y + c1->halfHeight;
	float z1 = cubeCenter.z - c1->halfThickness, z2 = cubeCenter.z + c1->halfThickness;

	if (sphereCenter.x>x1&&sphereCenter.x<x2)
	{
		if (sphereCenter.y>y1&&sphereCenter.y<y2)
		{
			if (c1->halfThickness + c2->radius > abs(cubeCenter.z - sphereCenter.z))
			{
				float zOffset = c1->halfThickness + c2->radius - abs(cubeCenter.z - sphereCenter.z);
				if (c2->velocity.z>0)
				{
					offset = XMFLOAT3(0.f, 0.f, -zOffset);
				}
				else
				{
					offset = XMFLOAT3(0.f, 0.f, zOffset);
				}
			}
		}
		if (sphereCenter.y <= y1&&sphereCenter.z <= z1)
		{
			if ((sphereCenter.y - y1)*(sphereCenter.y - y1) + (sphereCenter.z - z1)*(sphereCenter.z - z1)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(sphereCenter.x, y1 - c2->radius*cos(0.79f), z1 - c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.y <= y1&&sphereCenter.z >= z2)
		{
			if ((sphereCenter.y - y1)*(sphereCenter.y - y1) + (sphereCenter.z - z2)*(sphereCenter.z - z2)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(sphereCenter.x, y1 - c2->radius*cos(0.79f), z2 + c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.y >= y2&&sphereCenter.z <= z1)
		{
			if ((sphereCenter.y - y2)*(sphereCenter.y - y2) + (sphereCenter.z - z1)*(sphereCenter.z - z1)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(sphereCenter.x, y2 + c2->radius*cos(0.79f), z1 - c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.y >= y2&&sphereCenter.z >= z2)
		{
			if ((sphereCenter.y - y2)*(sphereCenter.y - y2) + (sphereCenter.z - z2)*(sphereCenter.z - z2)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(sphereCenter.x, y2 + c2->radius*cos(0.79f), z2 + c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
	}

	if (sphereCenter.y>y1&&sphereCenter.y<y2)
	{
		if (sphereCenter.z>z1&&sphereCenter.z<z2)
		{
			if (c1->halfWidth + c2->radius > abs(cubeCenter.x - sphereCenter.x))
			{
				float xOffset = c1->halfWidth + c2->radius - abs(cubeCenter.x - sphereCenter.x);
				if (c2->velocity.x>0)
				{
					offset = XMFLOAT3(-xOffset, 0.f, 0.f);
				}
				else
				{
					offset = XMFLOAT3(xOffset, 0.f, 0.f);
				}
			}
		}
		if (sphereCenter.z <= z1&&sphereCenter.x <= x1)
		{
			if ((sphereCenter.z - z1)*(sphereCenter.z - z1) + (sphereCenter.x - x1)*(sphereCenter.x - x1)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), sphereCenter.y, z1 - c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.z <= z1&&sphereCenter.x >= x2)
		{
			if ((sphereCenter.z - z1)*(sphereCenter.z - z1) + (sphereCenter.x - x2)*(sphereCenter.x - x2)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), sphereCenter.y, z1 - c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.z >= z2&&sphereCenter.x <= x1)
		{
			if ((sphereCenter.z - z2)*(sphereCenter.z - z2) + (sphereCenter.x - x1)*(sphereCenter.x - x1)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), sphereCenter.y, z2 + c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.z >= z2&&sphereCenter.x >= x2)
		{
			if ((sphereCenter.z - z2)*(sphereCenter.z - z2) + (sphereCenter.x - x2)*(sphereCenter.x - x2)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), sphereCenter.y, z2 + c2->radius*cos(0.79f), 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
	}

	if (sphereCenter.z>z1&&sphereCenter.z<z2)
	{
		if (sphereCenter.x>x1&&sphereCenter.x<x2)
		{
			if (c1->halfHeight + c2->radius > abs(cubeCenter.y - sphereCenter.y))
			{
				float yOffset = c1->halfHeight + c2->radius - abs(cubeCenter.y - sphereCenter.y);
				if (c2->velocity.y>0)
				{
					offset = XMFLOAT3(0.f, -yOffset, 0.f);
				}
				else
				{
					offset = XMFLOAT3(0.f, yOffset, 0.f);
				}
			}
		}
		if (sphereCenter.x <= x1&&sphereCenter.y <= y1)
		{
			if ((sphereCenter.x - x1)*(sphereCenter.x - x1) + (sphereCenter.y - y1)*(sphereCenter.y - y1)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), y1 - c2->radius*cos(0.79f), sphereCenter.z, 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.x <= x1&&sphereCenter.y >= y2)
		{
			if ((sphereCenter.x - x1)*(sphereCenter.x - x1) + (sphereCenter.y - y2)*(sphereCenter.y - y2)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), y2 + c2->radius*cos(0.79f), sphereCenter.z, 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.x >= x2&&sphereCenter.y <= y1)
		{
			if ((sphereCenter.x - x2)*(sphereCenter.x - x2) + (sphereCenter.y - y1)*(sphereCenter.y - y1)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), y1 - c2->radius*cos(0.79f), sphereCenter.z, 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
		if (sphereCenter.x >= x2&&sphereCenter.y >= y2)
		{
			if ((sphereCenter.x - x2)*(sphereCenter.x - x2) + (sphereCenter.y - y2)*(sphereCenter.y - y2)<(c2->radius*c2->radius))
			{
				XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), y2 + c2->radius*cos(0.79f), sphereCenter.z, 1.f);
				XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
				XMStoreFloat3(&offset, newPos - oldPos);
			}
		}
	}

	if (sphereCenter.x <= x1&&sphereCenter.y <= y1&&sphereCenter.z <= z1)
	{
		if ((sphereCenter.x - x1)*(sphereCenter.x - x1) + (sphereCenter.y - y1)*(sphereCenter.y - y1) + (sphereCenter.z - z1)*(sphereCenter.z - z1)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), y1 - c2->radius*cos(0.79f), z1 - c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	if (sphereCenter.x <= x1&&sphereCenter.y <= y1&&sphereCenter.z >= z2)
	{
		if ((sphereCenter.x - x1)*(sphereCenter.x - x1) + (sphereCenter.y - y1)*(sphereCenter.y - y1) + (sphereCenter.z - z2)*(sphereCenter.z - z2)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), y1 - c2->radius*cos(0.79f), z2 + c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	if (sphereCenter.x <= x1&&sphereCenter.y >= y2&&sphereCenter.z <= z1)
	{
		if ((sphereCenter.x - x1)*(sphereCenter.x - x1) + (sphereCenter.y - y2)*(sphereCenter.y - y2) + (sphereCenter.z - z1)*(sphereCenter.z - z1)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), y2 + c2->radius*cos(0.79f), z1 - c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	if (sphereCenter.x <= x1&&sphereCenter.y >= y2&&sphereCenter.z >= z2)
	{
		if ((sphereCenter.x - x1)*(sphereCenter.x - x1) + (sphereCenter.y - y2)*(sphereCenter.y - y2) + (sphereCenter.z - z2)*(sphereCenter.z - z2)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x1 - c2->radius*cos(0.79f), y2 + c2->radius*cos(0.79f), z2 + c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	if (sphereCenter.x >= x2&&sphereCenter.y <= y1&&sphereCenter.z <= z1)
	{
		if ((sphereCenter.x - x2)*(sphereCenter.x - x2) + (sphereCenter.y - y1)*(sphereCenter.y - y1) + (sphereCenter.z - z1)*(sphereCenter.z - z1)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), y1 - c2->radius*cos(0.79f), z1 - c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	if (sphereCenter.x >= x2&&sphereCenter.y <= y1&&sphereCenter.z >= z2)
	{
		if ((sphereCenter.x - x2)*(sphereCenter.x - x2) + (sphereCenter.y - y1)*(sphereCenter.y - y1) + (sphereCenter.z - z2)*(sphereCenter.z - z2)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), y1 - c2->radius*cos(0.79f), z2 + c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	if (sphereCenter.x >= x2&&sphereCenter.y >= y2&&sphereCenter.z <= z1)
	{
		if ((sphereCenter.x - x2)*(sphereCenter.x - x2) + (sphereCenter.y - y2)*(sphereCenter.y - y2) + (sphereCenter.z - z1)*(sphereCenter.z - z1)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), y2 + c2->radius*cos(0.79f), z1 - c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	if (sphereCenter.x >= x2&&sphereCenter.y >= y2&&sphereCenter.z >= z2)
	{
		if ((sphereCenter.x - x2)*(sphereCenter.x - x2) + (sphereCenter.y - y2)*(sphereCenter.y - y2) + (sphereCenter.z - z2)*(sphereCenter.z - z2)<(c2->radius*c2->radius))
		{
			XMVECTOR newPos = XMVectorSet(x2 + c2->radius*cos(0.79f), y2 + c2->radius*cos(0.79f), z2 + c2->radius*cos(0.79f), 1.f);
			XMVECTOR oldPos = XMLoadFloat3(&sphereCenter);
			XMStoreFloat3(&offset, newPos - oldPos);
		}
	}
	return offset;
}

XMFLOAT3 CollisionDetection::isOverlappedBetweenCylindricalAndSphere(CylindricalCollider * c1, SphericalCollider * c2)
{
	XMFLOAT3 offset(0.f, 0.f, 0.f);
	XMVECTOR cylinderLocalPosition = XMLoadFloat3(&(c1->position));
	XMMATRIX cylinderWorldTransform = XMLoadFloat4x4(&(c1->parentTransform));
	XMVECTOR cylinderWorldPosition = XMVector3TransformCoord(cylinderLocalPosition, cylinderWorldTransform);
	XMFLOAT3 cylinderCenter;
	XMStoreFloat3(&cylinderCenter, cylinderWorldPosition);
	XMVECTOR sphereLocalPosition = XMLoadFloat3(&(c2->position));
	XMMATRIX sphereWorldTransform = XMLoadFloat4x4(&(c2->parentTransform));
	XMVECTOR sphereWorldPosition = XMVector3TransformCoord(sphereLocalPosition, sphereWorldTransform);
	XMFLOAT3 sphereCenter;
	XMStoreFloat3(&sphereCenter, sphereWorldPosition);
	float centerDistance = XMVectorGetX(cylinderWorldPosition - sphereWorldPosition);
	float threshold = sqrt(c1->halfHeight*c1->halfHeight + c1->radius*c1->radius) + c2->radius;
	if (centerDistance >= threshold)
		return offset;

	if (sphereCenter.y>cylinderCenter.y - c1->halfHeight&&sphereCenter.y<cylinderCenter.y + c1->halfHeight)
	{
		if ((sphereCenter.x - cylinderCenter.x)*(sphereCenter.x - cylinderCenter.x) + (sphereCenter.z - cylinderCenter.z)*(sphereCenter.z - cylinderCenter.z)<c1->radius + c2->radius)
		{
			XMVECTOR centerDistance = sphereWorldPosition - XMVectorSet(cylinderCenter.x, sphereCenter.y, cylinderCenter.z, 1.f);
			XMVECTOR v = XMVector3Normalize(centerDistance)*(c1->radius + c2->radius - XMVectorGetX(XMVector3Length(centerDistance)));
			XMStoreFloat3(&offset, v);
		}
	}

	if (sphereCenter.y <= cylinderCenter.y - c1->halfHeight)
	{
		if ((sphereCenter.x - cylinderCenter.x)*(sphereCenter.x - cylinderCenter.x) + (sphereCenter.z - cylinderCenter.z)*(sphereCenter.z - cylinderCenter.z)<(c1->radius*c1->radius))
		{
			if (abs(sphereCenter.y - cylinderCenter.y)<c1->halfHeight + c2->radius)
			{
				XMVECTOR v = XMVectorSet(0.f, -1.f, 0.f, 0.f)*(c1->halfHeight + c2->radius - abs(sphereCenter.y - cylinderCenter.y));
				XMStoreFloat3(&offset, v);
			}
		}
		else
		{
			XMVECTOR bottomCenter = XMVectorSet(cylinderCenter.x, cylinderCenter.y - c1->halfHeight, cylinderCenter.z, 1.f);
			XMVECTOR edgePos = XMVector3Normalize(XMVectorSet(sphereCenter.x, cylinderCenter.y - c1->halfHeight, sphereCenter.z, 1.f) - bottomCenter)*c1->radius + bottomCenter;
			XMVECTOR v = sphereWorldPosition - edgePos;
			if (XMVectorGetX(XMVector3Length(v))<c2->radius)
			{
				XMVECTOR backoff = XMVector3Normalize(v)*(c2->radius - XMVectorGetX(XMVector3Length(v)));
				XMStoreFloat3(&offset, backoff);
			}
		}
	}

	if (sphereCenter.y >= cylinderCenter.y + c1->halfHeight)
	{
		if ((sphereCenter.x - cylinderCenter.x)*(sphereCenter.x - cylinderCenter.x) + (sphereCenter.z - cylinderCenter.z)*(sphereCenter.z - cylinderCenter.z)<(c1->radius*c1->radius))
		{
			if (abs(sphereCenter.y - cylinderCenter.y)<c1->halfHeight + c2->radius)
			{
				XMVECTOR v = XMVectorSet(0.f, 1.f, 0.f, 0.f)*(c1->halfHeight + c2->radius - abs(sphereCenter.y - cylinderCenter.y));
				XMStoreFloat3(&offset, v);
			}
		}
		else
		{
			XMVECTOR topCenter = XMVectorSet(cylinderCenter.x, cylinderCenter.y + c1->halfHeight, cylinderCenter.z, 1.f);
			XMVECTOR edgePos = XMVector3Normalize(XMVectorSet(sphereCenter.x, cylinderCenter.y + c1->halfHeight, sphereCenter.z, 1.f) - topCenter)*c1->radius + topCenter;
			XMVECTOR v = sphereWorldPosition - edgePos;
			if (XMVectorGetX(XMVector3Length(v))<c2->radius)
			{
				XMVECTOR backoff = XMVector3Normalize(v)*(c2->radius - XMVectorGetX(XMVector3Length(v)));
				XMStoreFloat3(&offset, backoff);
			}
		}
	}
	return offset;
}

bool CollisionDetection::isOverlapped(Collider *collider1, Collider *collider2)
{
	return false;
}