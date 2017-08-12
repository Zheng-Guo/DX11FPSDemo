#include "GameObject.h"

GameObject::GameObject(ID3D11Device * d) :device(d),
position(0.f, 0.f, 0.f),
rotation(0.f, 0.f, 0.f),
scaling(1.f, 1.f, 1.f),
transform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
collider(nullptr)
{

}

GameObject::~GameObject()
{
	for (Shape* s : shapes)
		delete s;
	delete collider;
}

void GameObject::setCollider(Collider * c)
{
	if (!collider)
	{
		delete collider;
	}
	collider = c;
	collider->setParentTransform(transform);
}

void GameObject::setPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
	for (Shape* s : shapes)
		s->setParentTransform(transform);
	if (collider)
		collider->setParentTransform(transform);
}

void GameObject::setRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
	for (Shape* s : shapes)
		s->setParentTransform(transform);
}

void GameObject::setScaling(float x, float y, float z)
{
	scaling = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
	for (Shape* s : shapes)
		s->setParentTransform(transform);
}

void GameObject::move(XMFLOAT3 offset)
{
	XMVECTOR oldPos = XMLoadFloat3(&position), posOffset = XMLoadFloat3(&offset);
	XMVECTOR newPos = oldPos + posOffset;
	XMStoreFloat3(&position, newPos);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
	for (Shape* s : shapes)
		s->setParentTransform(transform);
	if (collider)
		collider->setParentTransform(transform);
}

void GameObject::addShape(Shape * s)
{
	if (s)
	{
		s->setParentTransform(transform);
		shapes.push_back(s);
	}
}

void GameObject::update(float deltaTime)
{
}

void GameObject::draw(ID3D11DeviceContext * deviceContext, CXMMATRIX camView, CXMMATRIX camProjection)
{
	for (Shape* s : shapes)
		s->draw(deviceContext, camView, camProjection);
}
