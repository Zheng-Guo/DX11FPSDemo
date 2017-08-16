#pragma once
#include "Character.h"
#include "Camera.h"

class FirstPersonCharacter :public Character
{
protected:
	Camera camera;
	XMFLOAT3 cameraOffset, currentCameraOffset;
	float cameraPitch;
	float cameraPitchUpperLimit, cameraPitchLowerLimit;
public:
	FirstPersonCharacter(ID3D11Device* d);
	virtual ~FirstPersonCharacter();
	void setCameraOffset(float x, float y, float z);
	void rotateSight(float x, float y);
	virtual void move(XMFLOAT3 offset) override;
	virtual void update(float deltaTime) override;
	XMMATRIX getCamViewMatrix() { return camera.getCamViewMatrix(); }
	XMMATRIX getCamProjectionMatrix() const { return camera.getCamProjectionMatrix(); }
	XMFLOAT3 getCameraPosition()const { return camera.getPosition(); }
};