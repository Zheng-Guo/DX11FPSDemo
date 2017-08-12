#pragma once
#include "GameObject.h"

class Floor :public GameObject
{
public:
	Floor(ID3D11Device* d);
	void setMaterial(Material *m);
	virtual void setScaling(float x, float y, float z) override;
};