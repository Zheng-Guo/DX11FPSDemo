#include "ExplosionParticleSystem.h"

ExplosionParticleSystem::ExplosionParticleSystem(ID3D11Device * d) :ParticleSystem(d),
particleDuration(0.5f),
duration(0.5f),
particleDensity(10)
{
}

void ExplosionParticleSystem::update(float deltaTime)
{
	int i = 0;
	while (i<particles.size())
	{
		if (particles[i]->isActive())
		{
			particles[i]->update(deltaTime);
			++i;
		}
		else
		{
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
	}
	if (duration>0)
	{
		int numberOfParticlesToBeEmitted = particleDensity;
		XMFLOAT3 worldPosition;
		XMVECTOR wp = XMVector3TransformCoord(XMLoadFloat3(&position), XMLoadFloat4x4(&parentTransform));
		XMStoreFloat3(&worldPosition, wp);
		while (numberOfParticlesToBeEmitted>0)
		{
			Particle *p = new Particle(device);
			p->setMaterial(material);
			p->setScaling(0.1f, 0.1f, 0.1f);
			p->setDuration(particleDuration);
			p->setPosition(worldPosition.x, worldPosition.y, worldPosition.z);
			float randomPitch = (rand() % 628) / 100.f;
			float randomRoll = (rand() % 628) / 100.f;
			XMMATRIX orientation = XMMatrixRotationX(randomPitch)*XMMatrixRotationZ(randomRoll)*XMLoadFloat4x4(&parentTransform);
			XMVECTOR particleDirection = XMVector3Normalize(XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 1.f, 0.f), orientation));
			XMVECTOR velocity = particleDirection * (rand() % 10 + 5);
			p->setVelocity(XMVectorGetX(velocity), XMVectorGetY(velocity), XMVectorGetZ(velocity));
			particles.push_back(p);
			--numberOfParticlesToBeEmitted;
		}
		duration -= deltaTime;
	}
	if (duration <= 0 && particles.empty())
	{
		active = false;
	}
}
