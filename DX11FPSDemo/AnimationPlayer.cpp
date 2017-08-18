#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer():currentKeyFrameIndex(0),
accumulatedTime(0.f),
active(true)
{
	animationManager = AnimationManager::getInstance();
	currentAnimation = "walk";
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::addAnimation(string name)
{
	animations[name] = animationManager->getAnimation(name);
}

void AnimationPlayer::addSkeleton(Shape * s, string n)
{
	skeletons[s] = n;
	if(animations.find(n)!=animations.end())
	{
		s->setAnimationTransform(animations[n]->getAnimationSequence(n)[0]->transform);
	}
}

void AnimationPlayer::update(float deltaTime)
{
	accumulatedTime += deltaTime;
	if(currentKeyFrameIndex>=animations[currentAnimation]->getAnimationSequenceLength()-1)
	{
		accumulatedTime = 0;
		currentKeyFrameIndex = 0;
	}
	else
	{
		Animation *a = animations[currentAnimation];
		vector<KeyFrame*> as=a->getAnimationSequence(skeletons.begin()->second);
		if(accumulatedTime>=as[currentKeyFrameIndex+1]->timestamp)
		{
			++currentKeyFrameIndex;
			for(pair<Shape*,string> s:skeletons)
			{
				s.first->setAnimationTransform(a->getAnimationSequence(s.second)[currentKeyFrameIndex]->transform);
			}
		}
	}
}
