#include "FirstPersonCharacter.h"
#include "CollisionDetection.h"

FirstPersonCharacter::FirstPersonCharacter(ID3D11Device * d) :Character(d),
camera(XMFLOAT3(0.f, 0.f, 0.f), forwardDirection, upDirection),
cameraOffset(0.f, 0.f, 0.f),
currentCameraOffset(0.f, 0.f, 0.f),
cameraPitch(0),
cameraPitchUpperLimit(0.45f*3.14f),
cameraPitchLowerLimit(-0.45f*3.14f)
{
	GameObject::setCollider(new CylindricalCollider(2.f, 0.8f));
}

FirstPersonCharacter::~FirstPersonCharacter()
{
}

void FirstPersonCharacter::setCameraOffset(float x, float y, float z)
{
	cameraOffset = XMFLOAT3(x, y, z);
	currentCameraOffset = XMFLOAT3(x, y, z);
	camera.setPosition(position.x + cameraOffset.x, position.y + cameraOffset.y, position.z + cameraOffset.z);
}

void FirstPersonCharacter::rotateSight(float x, float y)
{
	cameraPitch += y;
	rotate(x);
	if (cameraPitch > cameraPitchUpperLimit)
		cameraPitch = cameraPitchUpperLimit;
	if (cameraPitch < cameraPitchLowerLimit)
		cameraPitch = cameraPitchLowerLimit;
	XMMATRIX camRotation = XMMatrixRotationRollPitchYaw(0, yaw, 0);
	XMVECTOR newCameraOffset = XMVector3TransformCoord(XMLoadFloat3(&cameraOffset), camRotation);
	XMStoreFloat3(&currentCameraOffset, newCameraOffset);
	camera.setPitch(cameraPitch);
	camera.setYaw(yaw);
}

void FirstPersonCharacter::move(XMFLOAT3 offset)
{
	Character::move(offset); 
	camera.setPosition(position.x + currentCameraOffset.x, position.y + currentCameraOffset.y, position.z + currentCameraOffset.z);
}

void FirstPersonCharacter::update(float deltaTime)
{
	Character::update(deltaTime);
	camera.setPosition(position.x + currentCameraOffset.x, position.y + currentCameraOffset.y, position.z + currentCameraOffset.z);
}
