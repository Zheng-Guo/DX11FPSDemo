#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ID3D11Device * d) :device(d),
position(0.f, 0.f, 0.f),
active(true),
parentTransform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
{
}

ParticleSystem::~ParticleSystem()
{
	for (Particle *p : particles)
		delete p;
}

void ParticleSystem::setMaterial(Material * m)
{
	material = m;
	for (Particle *p : particles)
		p->setMaterial(m);
}

void ParticleSystem::setColor(XMFLOAT4 c)
{
	color = c;
	for (Particle *p : particles)
		p->setColor(c.x, c.y, c.z, c.w);
}

void ParticleSystem::update(float deltaTime)
{
	for (Particle *p : particles)
		p->update(deltaTime);
}
