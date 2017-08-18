#include "AnimationPlayerManager.h"

AnimationPlayerManager* AnimationPlayerManager::instance = nullptr;

AnimationPlayerManager::AnimationPlayerManager()
{	
}

AnimationPlayerManager::~AnimationPlayerManager()
{
	for (AnimationPlayer* p : animationPlayers)
		delete p;
}

AnimationPlayerManager * AnimationPlayerManager::getInstance()
{
	if(!instance)
	{
		instance = new AnimationPlayerManager;
	}
	return instance;
}

void AnimationPlayerManager::addAnimationPlayer(AnimationPlayer * a)
{
	animationPlayers.push_back(a);
}

void AnimationPlayerManager::update(float deltaTime)
{
	int i = 0;
	while(i<animationPlayers.size())
	{
		if(animationPlayers[i]->isActive())
		{
			animationPlayers[i]->update(deltaTime);
			++i;
		}
		else
		{
			delete animationPlayers[i];
			animationPlayers.erase(animationPlayers.begin() + i);
		}
	}
}

void AnimationPlayerManager::reset()
{
	for (AnimationPlayer* p : animationPlayers)
		delete p;
	animationPlayers.clear();
}
