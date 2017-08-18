#pragma once
#include <d3d11.h>
#include <xnamath.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

struct KeyFrame
{
	float timestamp;
	XMFLOAT4X4 transform;
};

class Animation
{
	map<string,vector<KeyFrame*>> animationSequences;
public:
	Animation(){}
	~Animation();
	void loadAnimation(const char *filename);
	int getAnimationSequenceLength();
	vector<KeyFrame*> getAnimationSequence(string name);
};
