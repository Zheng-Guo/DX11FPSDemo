#include "Animation.h"
#include <fstream>
using namespace std;

Animation::~Animation()
{
	for(pair<string,vector<KeyFrame*>> a:animationSequences)
	{
		for (KeyFrame *k : a.second)
			delete k;
	}
}

void Animation::loadAnimation(const char * filename)
{
	ifstream ifs(filename);
	for (pair<string, vector<KeyFrame*>> a : animationSequences)
	{
		for (KeyFrame *k : a.second)
			delete k;
	}
	animationSequences.clear();
	string name;
	float posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ;
	while(true)
	{	
		ifs >> name;
		if (ifs.eof())
			break;
		vector<KeyFrame*> sequence;
		float f;
		ifs >> f;
		while(f!=-1)
		{
			KeyFrame *keyFrame = new KeyFrame;
			keyFrame->timestamp = f;
			ifs >> posX >> posY >> posZ;
			ifs >> rotX >> rotY >> rotZ;
			ifs >> scaX >> scaY >> scaZ;
			XMMATRIX aniTransform = XMMatrixScaling(scaX,scaY,scaZ)*XMMatrixRotationX(rotX)*XMMatrixRotationY(rotY)*XMMatrixRotationZ(rotZ)*XMMatrixTranslation(posX,posY,posZ);
			XMStoreFloat4x4(&(keyFrame->transform), aniTransform);
			sequence.push_back(keyFrame);
			ifs >> f;
		}
		animationSequences[name] = sequence;
	}
	ifs.close();
}

vector<KeyFrame*> Animation::getAnimationSequence(string name)
{
	return animationSequences[name];
}
