#pragma once
#include <vector>
#include "AnimationPlayer.h"

using namespace std;

class AnimationPlayerManager
{
	static AnimationPlayerManager* instance;
	vector<AnimationPlayer*> animationPlayers;
	AnimationPlayerManager();
public:
	~AnimationPlayerManager();
	static AnimationPlayerManager* getInstance();
	void addAnimationPlayer(AnimationPlayer* a);
	void update(float deltaTime);
};