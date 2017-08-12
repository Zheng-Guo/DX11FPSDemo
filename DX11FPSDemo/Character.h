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
	bool isRotatingLeft, isRotatingRight;
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
	void rotateLeft() { isRotatingLeft = true; }
	void rotateRight() { isRotatingRight = true; }
	void rotate(float deltaYaw);
	virtual void update(float deltaTime) override;
};
