#include "ProjectileManager.h"

ProjectileManager* ProjectileManager::instance = nullptr;

ProjectileManager::~ProjectileManager()
{
	for (int i = 0; i < projectiles.size(); ++i)
		delete projectiles[i];
}

ProjectileManager * ProjectileManager::getInstance()
{
	if (!instance)
	{
		instance = new ProjectileManager;
	}
	return instance;
}

void ProjectileManager::addProjectile(Fireball * f)
{
	f->setMaterial(material);
	projectiles.push_back(f);
}

void ProjectileManager::update(float deltaTime)
{
	int i = 0;
	while (i<projectiles.size())
	{
		if (projectiles[i]->isActive())
		{
			projectiles[i]->update(deltaTime);
			++i;
		}
		else
		{
			delete projectiles[i];
			projectiles.erase(projectiles.begin() + i);
		}
	}
}

void ProjectileManager::reset()
{
	for (Fireball *f : projectiles)
		delete f;
	projectiles.clear();
}
