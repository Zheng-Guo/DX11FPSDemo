#pragma once
#include "Animation.h"

class AnimationManager
{
	map<string, Animation*> animations;
	static AnimationManager* instance;
	AnimationManager();
public:
	static AnimationManager* getInstance();
	~AnimationManager();
	void loadAnimation(string name,const char* filename);
	Animation* getAnimation(string name);
};