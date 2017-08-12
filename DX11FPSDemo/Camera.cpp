#include "Camera.h"

Camera::Camera(XMFLOAT3 pos, XMFLOAT3 t, XMFLOAT3 u, float p, float y, float aov, float ar, float nz, float fz) :position(pos),
targetDirection(t),
facingDirection(t),
up(u),
pitch(p),
yaw(y),
angleOfView(aov),
aspectRatio(ar),
nearZ(nz),
farZ(fz)
{

}

void Camera::move(const XMVECTOR& translation)
{
	XMVECTOR newPos = XMLoadFloat3(&position) + translation;
	XMStoreFloat3(&position, newPos);
}

void Camera::rotate(float p, float y)
{
	pitch += p;
	yaw += y;
	if (pitch < 0)
		pitch += 6.28f;
	if (pitch >= 6.28f)
		pitch -= 6.28;
	if (yaw < 0)
		yaw += 6.28f;
	if (yaw >= 6.28f)
		yaw -= 6.28f;
}

XMMATRIX Camera::getCamViewMatrix()
{
	XMMATRIX camRotation = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);
	XMVECTOR currentDirection = XMVector3Normalize(XMVector3TransformCoord(XMLoadFloat3(&targetDirection), camRotation));
	XMStoreFloat3(&facingDirection, currentDirection);
	return XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&position) + currentDirection, XMLoadFloat3(&up));
}

XMMATRIX Camera::getCamProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(angleOfView, aspectRatio, nearZ, farZ);
}

XMFLOAT3 Camera::getPosition() const
{
	return position;
}
