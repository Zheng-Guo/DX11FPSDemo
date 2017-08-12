#pragma once
#include "Camera.h"

class FreeLookCamera : public Camera
{
protected:
	XMFLOAT3 forwardDirection;
	XMFLOAT3 leftDirection;
	float linearSpeed;
	float angularSpeed;
public:
	FreeLookCamera(XMFLOAT3 pos, XMFLOAT3 t, XMFLOAT3 u, float ls = 1.f, float as = 0.001f, float p = 0, float y = 0, float aov = 0.2f*3.14f, float ar = 1.f, float nz = 1.f, float fz = 1000.f);
	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveLeftward(float deltaTime);
	void moveRightward(float deltaTime);
	virtual void rotate(float p, float y) override;
};