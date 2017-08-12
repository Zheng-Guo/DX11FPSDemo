#pragma once
#include "GameObject.h"

class WallBlock :public GameObject
{
public:
	WallBlock(ID3D11Device* d);
	void setMaterial(Material *m);
	virtual void setScaling(float x, float y, float z) override;
};