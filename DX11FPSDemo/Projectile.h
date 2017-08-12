#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
protected:
	XMFLOAT3 velocity;
	float damage;
	float duration;
	bool active;
public:
	Projectile(ID3D11Device* d);
	void setVelocity(XMFLOAT3 v);
	void setDamage(float d) { damage = d; }
	float getDamage()const { return damage; }
	void setDuration(float d) { duration = d; }
	void setActive(bool a) { active = a; }
	bool isActive() const { return active; }
	virtual void update(float deltaTime) override;
};