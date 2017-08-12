#include "ParticleSystemManager.h"

ParticleSystemManager* ParticleSystemManager::instance = nullptr;

ParticleSystemManager::~ParticleSystemManager()
{
	for (ParticleSystem* p : particleSystems)
		delete p;
}

ParticleSystemManager * ParticleSystemManager::getInstance()
{
	if (!instance)
	{
		instance = new ParticleSystemManager;
	}
	return instance;
}

void ParticleSystemManager::addParticleSystem(ParticleSystem * p)
{
	particleSystems.push_back(p);
}

void ParticleSystemManager::update(float deltaTime)
{
	int i = 0;
	while (i<particleSystems.size())
	{
		if (particleSystems[i]->isActive())
		{
			particleSystems[i]->update(deltaTime);
			++i;
		}
		else
		{
			delete particleSystems[i];
			particleSystems.erase(particleSystems.begin() + i);
		}
	}
}

void ParticleSystemManager::reset()
{
	for (ParticleSystem *p : particleSystems)
		delete p;
	particleSystems.clear();
}
