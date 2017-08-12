#include "Collider.h"

Collider::Collider(ColliderType c) :colliderType(c),
position(0.f, 0.f, 0.f),
rotation(0.f, 0.f, 0.f),
scaling(1.f, 1.f, 1.f),
transform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
parentTransform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
velocity(0.f, 0.f, 0.f)
{
}

void Collider::setPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void Collider::setRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void Collider::setScaling(float x, float y, float z)
{
	scaling = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

