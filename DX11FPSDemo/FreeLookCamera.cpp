#include "FreeLookCamera.h"

FreeLookCamera::FreeLookCamera(XMFLOAT3 pos, XMFLOAT3 t, XMFLOAT3 u, float ls, float as, float p, float y, float aov, float ar, float nz, float fz) :Camera(pos, t, u, p, y, aov, ar, nz, fz),
linearSpeed(ls),
angularSpeed(as)
{
	XMVECTOR forward = XMVector3Normalize(XMLoadFloat3(&t));
	XMStoreFloat3(&forwardDirection, forward);
	XMVECTOR left = XMVector3Normalize(XMVector3Cross(forward, XMLoadFloat3(&up)));
	XMStoreFloat3(&leftDirection, left);
}

void FreeLookCamera::moveForward(float deltaTime)
{
	XMVECTOR pos = XMLoadFloat3(&position);
	pos += XMLoadFloat3(&forwardDirection)*linearSpeed*deltaTime;
	XMStoreFloat3(&position, pos);
}

void FreeLookCamera::moveBackward(float deltaTime)
{
	XMVECTOR pos = XMLoadFloat3(&position);
	pos -= XMLoadFloat3(&forwardDirection)*linearSpeed*deltaTime;
	XMStoreFloat3(&position, pos);
}

void FreeLookCamera::moveLeftward(float deltaTime)
{
	XMVECTOR pos = XMLoadFloat3(&position);
	pos += XMLoadFloat3(&leftDirection)*linearSpeed*deltaTime;
	XMStoreFloat3(&position, pos);
}

void FreeLookCamera::moveRightward(float deltaTime)
{
	XMVECTOR pos = XMLoadFloat3(&position);
	pos -= XMLoadFloat3(&leftDirection)*linearSpeed*deltaTime;
	XMStoreFloat3(&position, pos);
}

void FreeLookCamera::rotate(float p, float y)
{
	Camera::rotate(p*angularSpeed, y*angularSpeed);
	XMMATRIX camRotation = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);
	XMVECTOR defaultForward = XMVector3Normalize(XMLoadFloat3(&targetDirection));
	XMVECTOR newForward = XMVector3Normalize(XMVector3TransformCoord(defaultForward, camRotation));
	XMVECTOR newUp = XMVector3TransformCoord(XMLoadFloat3(&up), camRotation);
	XMVECTOR newLeft = XMVector3Normalize(XMVector3Cross(newForward, newUp));
	XMStoreFloat3(&forwardDirection, newForward);
	XMStoreFloat3(&leftDirection, newLeft);
}


