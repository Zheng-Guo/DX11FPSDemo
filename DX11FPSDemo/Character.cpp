#include "Character.h"

Character::Character(ID3D11Device * d) :GameObject(d),
defaultForward(0.f, 0.f, 1.f),
forwardDirection(0.f, 0.f, 1.f),
leftDirection(-1.0f, 0.0f, 0.0f),
upDirection(0.0f, 1.0f, 0.0f),
yaw(0.f),
linearSpeed(1.f),
angularSpeed(3.14f),
isMovingForward(false),
isMovingBackward(false),
isMovingLeftward(false),
isMovingRightward(false)
{
}

Character::~Character()
{
}

void Character::setRotation(float x, float y, float z)
{
	GameObject::setRotation(x, y, z);
	XMVECTOR defaultFor = XMLoadFloat3(&defaultForward);
	XMVECTOR forward = XMLoadFloat3(&forwardDirection);
	XMVECTOR left = XMLoadFloat3(&leftDirection);
	XMVECTOR up = XMLoadFloat3(&upDirection);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(x, y, z);
	XMVECTOR newDefaultForward = XMVector3Normalize(XMVector3TransformCoord(defaultFor, rotationMatrix));
	XMVECTOR newForward = XMVector3Normalize(XMVector3TransformCoord(forward, rotationMatrix));
	XMVECTOR newLeft = XMVector3Normalize(XMVector3TransformCoord(left, rotationMatrix));
	XMVECTOR newUp = XMVector3Normalize(XMVector3TransformCoord(up, rotationMatrix));
	XMStoreFloat3(&defaultForward, newDefaultForward);
	XMStoreFloat3(&forwardDirection, newForward);
	XMStoreFloat3(&leftDirection, newLeft);
	XMStoreFloat3(&upDirection, newUp);
	yaw = 0;
}

void Character::rotate(float deltaYaw)
{
	yaw += deltaYaw;
	if (yaw > 3.14f)
		yaw -= 6.28f;
	if (yaw < -3.14f)
		yaw += 6.28f;
	GameObject::setRotation(0.f, yaw, 0.f);
	XMMATRIX characterRotation = XMMatrixRotationRollPitchYaw(0.f, yaw, 0.f);
	XMVECTOR newForwardDirection = XMVector3Normalize(XMVector3TransformCoord(XMLoadFloat3(&defaultForward), characterRotation));
	XMVECTOR newLeftDirection = XMVector3Normalize(XMVector3Cross(newForwardDirection, XMLoadFloat3(&upDirection)));
	XMStoreFloat3(&forwardDirection, newForwardDirection);
	XMStoreFloat3(&leftDirection, newLeftDirection);
}

void Character::move(XMFLOAT3 offset)
{
	GameObject::move(offset);
}

void Character::update(float deltaTime)
{
	XMFLOAT3 newPos = position, oldPos = position;
	if (isMovingForward)
	{
		XMVECTOR pos = XMLoadFloat3(&position);
		pos += XMLoadFloat3(&forwardDirection)*linearSpeed*deltaTime;
		XMStoreFloat3(&newPos, pos);
		GameObject::setPosition(newPos.x, newPos.y, newPos.z);
	}
	if (isMovingBackward)
	{
		XMVECTOR pos = XMLoadFloat3(&position);
		pos -= XMLoadFloat3(&forwardDirection)*linearSpeed*deltaTime;
		XMStoreFloat3(&newPos, pos);
		GameObject::setPosition(newPos.x, newPos.y, newPos.z);
	}
	if (isMovingLeftward)
	{
		XMVECTOR pos = XMLoadFloat3(&position);
		pos += XMLoadFloat3(&leftDirection)*linearSpeed*deltaTime;
		XMStoreFloat3(&newPos, pos);
		GameObject::setPosition(newPos.x, newPos.y, newPos.z);
	}
	if (isMovingRightward)
	{
		XMVECTOR pos = XMLoadFloat3(&position);
		pos -= XMLoadFloat3(&leftDirection)*linearSpeed*deltaTime;
		XMStoreFloat3(&newPos, pos);
		GameObject::setPosition(newPos.x, newPos.y, newPos.z);
	}
	isMovingForward = isMovingBackward = isMovingLeftward = isMovingRightward = false;
	XMVECTOR p1 = XMLoadFloat3(&oldPos), p2 = XMLoadFloat3(&newPos);
	XMVECTOR velocity = (p2 - p1) / deltaTime;
	XMFLOAT3 v;
	XMStoreFloat3(&v, velocity);
	collider->setVelocity(v);
}
