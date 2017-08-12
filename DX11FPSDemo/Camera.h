#pragma once
#include <d3d11.h>
#include <xnamath.h>

class Camera
{
protected:
	XMFLOAT3 position;
	XMFLOAT3 targetDirection;
	XMFLOAT3 up;
	XMFLOAT3 facingDirection;
	float pitch, yaw;
	float angleOfView, aspectRatio, nearZ, farZ;
public:
	Camera(XMFLOAT3 pos, XMFLOAT3 t, XMFLOAT3 u, float p = 0, float y = 0, float aov = 0.2f*3.14f, float ar = 1.f, float nz = 1.f, float fz = 1000.f);
	void move(const XMVECTOR& translation);
	virtual void rotate(float p, float y);
	void setPosition(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	void setTargetDirection(float x, float y, float z) { targetDirection = XMFLOAT3(x, y, z); }
	void setUp(float x, float y, float z) { up = XMFLOAT3(x, y, z); }
	void setPitch(float p) { pitch = p; }
	void setYaw(float y) { yaw = y; }
	XMMATRIX getCamViewMatrix();
	XMMATRIX getCamProjectionMatrix() const;
	XMFLOAT3 getPosition() const;
	XMFLOAT3 getFacingDirection() const { return facingDirection; }
};