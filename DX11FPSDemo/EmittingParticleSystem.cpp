#include "EmittingParticleSystem.h"
#include <cstdlib>

EmittingParticleSystem::EmittingParticleSystem(ID3D11Device * d) :ParticleSystem(d),
direction(0.f, 0.f, 0.f),
particleDuration(0.2f),
spreadAngle(3.14f / 18),
particleDensity(20)
{

}

void EmittingParticleSystem::update(float deltaTime)
{
	int numberOfParticlesToBeEmitted = particleDensity;
	vector<Particle*>::iterator ite = particles.begin();
	XMFLOAT3 worldPosition;
	XMVECTOR wp = XMVector3TransformCoord(XMLoadFloat3(&position), XMLoadFloat4x4(&parentTransform));
	XMStoreFloat3(&worldPosition, wp);
	while (ite != particles.end())
	{
		if ((*ite)->isActive())
		{
			(*ite)->update(deltaTime);
		}
		else
		{
			(*ite)->setDuration(particleDuration);
			(*ite)->setPosition(worldPosition.x, worldPosition.y, worldPosition.z);
			int spreadRange = spreadAngle * 200;
			float randomPitch = (rand() % spreadRange - spreadRange / 2) / 100.f;
			float randomRoll = (rand() % 628) / 100.f;
			XMMATRIX orientation = XMMatrixRotationX(randomPitch)*XMMatrixRotationZ(randomRoll)*XMMatrixRotationX(direction.x)*XMMatrixRotationY(direction.y)*XMMatrixRotationZ(direction.z)*XMLoadFloat4x4(&parentTransform);
			XMVECTOR particleDirection = XMVector3Normalize(XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 1.f, 0.f), orientation));
			XMVECTOR velocity = particleDirection * (rand() % 3 + 5);
			(*ite)->setVelocity(XMVectorGetX(velocity), XMVectorGetY(velocity), XMVectorGetZ(velocity));
			--numberOfParticlesToBeEmitted;
		}
		++ite;
	}
	while (numberOfParticlesToBeEmitted>0)
	{
		Particle *p = new Particle(device);
		p->setScaling(0.01f, 0.01f, 0.01f);
		p->setMaterial(material);
		p->setDuration(particleDuration);
		p->setPosition(worldPosition.x, worldPosition.y, worldPosition.z);
		int spreadRange = spreadAngle * 200;
		float randomPitch = (rand() % spreadRange - spreadRange / 2) / 100.f;
		float randomRoll = (rand() % 628) / 100.f;
		XMMATRIX orientation = XMMatrixRotationX(randomPitch)*XMMatrixRotationZ(randomRoll)*XMMatrixRotationX(direction.x)*XMMatrixRotationY(direction.y)*XMMatrixRotationZ(direction.z)*XMLoadFloat4x4(&parentTransform);
		XMVECTOR particleDirection = XMVector3Normalize(XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 1.f, 0.f), orientation));
		XMVECTOR velocity = particleDirection * (rand() % 3 + 5);
		p->setVelocity(XMVectorGetX(velocity), XMVectorGetY(velocity), XMVectorGetZ(velocity));
		//p->setColor(color.x,color.y,color.z,color.w);
		particles.push_back(p);
		--numberOfParticlesToBeEmitted;
	}
}
