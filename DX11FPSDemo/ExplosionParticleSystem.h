#pragma once
#include "ParticleSystem.h"

class ExplosionParticleSystem :public ParticleSystem
{
protected:
	float particleDuration;
	float duration;
	int particleDensity;
public:
	ExplosionParticleSystem(ID3D11Device* d);
	void setParticleDuration(float p) { particleDuration = p; }
	void setDuration(float d) { duration = d; }
	void setParticleDensity(int p) { particleDensity = p; }
	virtual void update(float deltaTime) override;
};