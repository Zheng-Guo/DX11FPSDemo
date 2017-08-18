#pragma once
#include "GameObject.h"

class Character : public GameObject
{
protected:
	XMFLOAT3 defaultForward;
	XMFLOAT3 forwardDirection;
	XMFLOAT3 leftDirection;
	XMFLOAT3 upDirection;
	float yaw;
	float linearSpeed;
	float angularSpeed;
	bool isMovingForward, isMovingBackward, isMovingLeftward, isMovingRightward;
public:
	Character(ID3D11Device* d);
	virtual ~Character();
	void setLinearSpeed(float l) { linearSpeed = l; }
	void setAngularSpeed(float a) { angularSpeed = a; }
	virtual void setRotation(float x, float y, float z) override;
	void moveForward() { isMovingForward = true; }
	void moveBackward() { isMovingBackward = true; }
	void moveLeftward() { isMovingLeftward = true; }
	void moveRightward() { isMovingRightward = true; }
	void rotate(float deltaYaw);
	bool isMoving()const { return isMovingForward || isMovingBackward || isMovingLeftward || isMovingRightward; }
	virtual void move(XMFLOAT3 offset) override;
	virtual void update(float deltaTime) override;
};
