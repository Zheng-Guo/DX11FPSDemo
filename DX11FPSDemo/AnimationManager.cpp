#include "AnimationManager.h"

AnimationManager* AnimationManager::instance = nullptr;

AnimationManager::AnimationManager()
{
	
}


AnimationManager * AnimationManager::getInstance()
{
	if(!instance)
	{
		instance = new AnimationManager;
	}
	return instance;
}

AnimationManager::~AnimationManager()
{
	for (pair<string, Animation*> p : animations)
		delete p.second;
}

void AnimationManager::loadAnimation(string name,const char * filename)
{
	Animation *a = new Animation;
	a->loadAnimation(filename);
	animations[name] = a;
}

Animation * AnimationManager::getAnimation(string name)
{
	return animations[name];
}
