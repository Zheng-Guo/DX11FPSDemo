#pragma once
#include "ParticleSystem.h"

class EmittingParticleSystem :public ParticleSystem
{
protected:
	XMFLOAT3 direction;
	float particleDuration;
	float spreadAngle;
	int particleDensity;
public:
	EmittingParticleSystem(ID3D11Device* d);
	void setDirection(XMFLOAT3 d) { direction = d; }
	void setSpreadAngle(float a) { spreadAngle = a; }
	void setParticleDensity(int p) { particleDensity = p; }
	void setParticleDuration(float p) { particleDuration = p; }
	virtual void update(float deltaTime) override;
};