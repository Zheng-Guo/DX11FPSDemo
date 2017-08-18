#pragma once
#include "AnimationManager.h"
#include "EnemyCharacter.h"

class AnimationPlayer
{
	map<Shape*, string> skeletons;
	map<string, Animation*> animations;
	AnimationManager *animationManager;
	int currentKeyFrameIndex;
	float accumulatedTime;
	string currentAnimation;
	EnemyCharacter* animatedCharacter;
	bool active;
public:
	AnimationPlayer();
	~AnimationPlayer();
	void setAnimatedCharacter(EnemyCharacter* c) { animatedCharacter = c; }
	void addAnimation(string name);
	void addSkeleton(Shape* s, string n);
	void setActive(bool b) { active = b; }
	bool isActive()const { return active; }
	void update(float deltaTime);
};