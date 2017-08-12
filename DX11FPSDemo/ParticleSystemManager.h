#pragma once
#include "ParticleSystem.h"

class ParticleSystemManager
{
	vector<ParticleSystem*> particleSystems;
	static ParticleSystemManager* instance;
public:
	~ParticleSystemManager();
	static ParticleSystemManager* getInstance();
	void addParticleSystem(ParticleSystem *p);
	vector<ParticleSystem*>::iterator getListBegin() { return particleSystems.begin(); }
	vector<ParticleSystem*>::iterator getListEnd() { return particleSystems.end(); }
	void update(float deltaTime);
	void reset();
};