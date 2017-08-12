#pragma once
#include "Projectile.h"

class Fireball :public Projectile
{
protected:
	float radius;
public:
	Fireball(ID3D11Device* d, float r);
	void setMaterial(Material * m);
	virtual void update(float deltaTime);
};