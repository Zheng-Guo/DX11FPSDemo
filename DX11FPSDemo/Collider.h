#pragma once
#include <d3d11.h>
#include <xnamath.h>

class CollisionDetection;

class Collider
{
public:
	enum ColliderType { Cubic, Cylindrical, Spherical };
protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scaling;
	XMFLOAT4X4 transform;
	XMFLOAT4X4 parentTransform;
	ColliderType colliderType;
	XMFLOAT3 velocity;
	friend class CollisionDetection;
public:
	Collider(ColliderType c);
	ColliderType getType() const { return colliderType; }
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScaling(float x, float y, float z);
	void setParentTransform(XMFLOAT4X4 p) { parentTransform = p; }
	void setVelocity(XMFLOAT3 v) { velocity = v; }
	XMFLOAT3 getPosition()const { return position; }
	XMFLOAT3 getRotation()const { return rotation; }
	XMFLOAT3 getScaling() const { return scaling; }
	XMFLOAT3 getVelocity()const { return velocity; }
};