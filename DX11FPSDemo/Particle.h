#pragma once
#include "GameObject.h"

class Particle :public GameObject
{
protected:
	XMFLOAT3 velocity;
	XMFLOAT3 acceleration;
	float duration;
	XMFLOAT4 color;
public:
	Particle(ID3D11Device* device);
	void setVelocity(float x, float y, float z);
	void setAcceleration(float x, float y, float z);
	void setDuration(float d) { duration = d; }
	void setColor(float r, float g, float b, float a);
	void setMaterial(Material *m);
	void setAmbientLight(float r, float g, float b, float a);
	bool isActive() const { return duration > 0; }
	virtual void update(float deltaTime) override;
};