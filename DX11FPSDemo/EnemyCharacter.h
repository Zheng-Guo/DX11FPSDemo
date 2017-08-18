#pragma once
#include "Character.h"
#include "Material.h"
#include "EmittingParticleSystem.h"
#include "ParticleSystemManager.h"
#include "PlayerCharacter.h"

class AIController;
class AnimationPlayer;

class EnemyCharacter :public Character
{
protected:
	float life;
	float damage;
	float wonderRange;
	float guardRange;
	float fireRange;
	bool isAttacking;
	AIController *controller;
	PlayerCharacter *playerCharacter;
	Material *weaponMaterial;
	EmittingParticleSystem *weapon;
	ParticleSystemManager* particleSystemManager;
	AnimationPlayer* animationPlayer;
public:
	EnemyCharacter(ID3D11Device* d);
	virtual ~EnemyCharacter();
	void setMaterial(Material *m);
	void setWeaponMaterial(Material *m) { weaponMaterial = m; }
	void setWaitTime(float w);
	void setPatrolling(bool b);
	void addPatrolTarget(int x, int z);
	void setLife(float l) { life = l; }
	void setDamage(float d) { damage = d; }
	void takeDamage(float d) { life -= d; }
	void attack();
	void stopAttacking();
	void setWonderRange(float w) { wonderRange = w; }
	float getWonderRange() const { return wonderRange; }
	void setGuardRange(float g) { guardRange = g; }
	float getGuardRange()const { return guardRange; }
	void setFireRange(float f) { fireRange = f; }
	float getFireRange()const { return fireRange; }
	void setPlayerCharacter(PlayerCharacter *c) { playerCharacter = c; }
	Character* getPlayerCharacter()const { return playerCharacter; }
	XMFLOAT3 getForwardDirection() const { return forwardDirection; }
	XMFLOAT3 getUpDirection() const { return upDirection; }
	bool isActive()const { return life > 0; }
	virtual void update(float deltaTime) override;
};