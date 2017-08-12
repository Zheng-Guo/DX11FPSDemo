#include "AIManager.h"

AIManager* AIManager::instance = nullptr;

AIManager::AIManager()
{
}

AIManager * AIManager::getInstance()
{
	if (!instance)
	{
		instance = new AIManager;
	}
	return instance;
}

AIManager::~AIManager()
{
	for (AIController *c : controllers)
		delete c;
}

void AIManager::addController(AIController * c)
{
	controllers.push_back(c);
}

void AIManager::update(float deltaTime)
{
	int i = 0;
	while (i<controllers.size())
	{
		if (controllers[i]->isActive())
		{
			controllers[i]->update(deltaTime);
			++i;
		}
		else
		{
			delete controllers[i];
			controllers.erase(controllers.begin() + i);
		}
	}
}

void AIManager::reset()
{
	for (AIController *c : controllers)
		delete c;
	controllers.clear();
}
