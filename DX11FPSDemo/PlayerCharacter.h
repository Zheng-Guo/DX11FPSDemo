#pragma once
#include "FirstPersonCharacter.h"
#include "ProjectileManager.h"

class PlayerCharacter :public FirstPersonCharacter
{
protected:
	float life, totalLife, mana, totalMana, manaRecoveryRate;
	int score;
	float fireballSpeed, fireballDamage, fireballManaCost;
	XMFLOAT3 projectileOffset;
	bool fire;
	ProjectileManager *projectileManager;
public:
	PlayerCharacter(ID3D11Device* d);
	void setMaterial(Material * m);
	void setProjectileOffset(XMFLOAT3 p) { projectileOffset = p; }
	void setFireballSpeed(float s) { fireballSpeed = s; }
	void setFireballDamage(float d) { fireballDamage = d; }
	void setLife(float l) { life = l; totalLife = l; }
	float getLifePercentage()const { return life / totalLife; }
	void setMana(float m) { mana = m; totalMana = m; }
	void setManaRecoveryRate(float m) { manaRecoveryRate = m; }
	float getManaPercentage()const { return mana / totalMana; }
	void setFireballManaCost(float f) { fireballManaCost = f; }
	void takeDamage(float d);
	void fireFireball();
	void resetOrientation();
	virtual void update(float deltaTime) override;
};