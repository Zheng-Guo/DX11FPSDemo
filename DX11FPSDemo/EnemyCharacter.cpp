#include "EnemyCharacter.h"
#include "CollisionDetection.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"
#include "AIManager.h"

EnemyCharacter::EnemyCharacter(ID3D11Device * d) :Character(d),
life(3.f),
damage(2.f),
wonderRange(16.f),
guardRange(16.f),
fireRange(5.f),
isAttacking(false),
weapon(nullptr)
{
	AIManager* aiManager = AIManager::getInstance();
	controller = new AIController;
	controller->setControlledCharacter(this);
	aiManager->addController(controller);

	particleSystemManager = ParticleSystemManager::getInstance();

	GameObject::setCollider(new CylindricalCollider(2.f, 2.f));

	Sphere *head = new Sphere(d);
	head->setScale(0.4f, 0.4f, 0.4f);
	head->setPosition(0.f, 1.6f, 0.f);
	GameObject::addShape(head);

	Cube *eye = new Cube(d);
	eye->setScale(0.3f, 0.1f, 0.3f);
	eye->setPosition(0.f, 1.6f, 0.2f);
	GameObject::addShape(eye);

	Cylinder *body = new Cylinder(d);
	body->setScale(0.5f, 0.8f, 0.5f);
	body->setPosition(0.f, 0.4f, 0.f);
	GameObject::addShape(body);

	Cube *leftArm = new Cube(d);
	leftArm->setScale(0.15f, 0.8f, 0.15f);
	leftArm->setPosition(-0.7f, 0.4f, 0.f);
	GameObject::addShape(leftArm);

	Cube *rightArm = new Cube(d);
	rightArm->setScale(0.15f, 0.8f, 0.15f);
	rightArm->setPosition(0.7f, 0.4f, 0.f);
	GameObject::addShape(rightArm);

	Cube *leftLeg = new Cube(d);
	leftLeg->setScale(0.2f, 0.8f, 0.2f);
	leftLeg->setPosition(-0.3f, -1.2f, 0.f);
	GameObject::addShape(leftLeg);

	Cube *rightLeg = new Cube(d);
	rightLeg->setScale(0.2f, 0.8f, 0.2f);
	rightLeg->setPosition(0.3f, -1.2f, 0.f);
	GameObject::addShape(rightLeg);
}

EnemyCharacter::~EnemyCharacter()
{
	controller->setActive(false);
	if (weapon)
	{
		weapon->setActive(false);
	}
}

void EnemyCharacter::setMaterial(Material * m)
{
	for (Shape *s : GameObject::shapes)
		s->setMaterial(m);
}

void EnemyCharacter::setWaitTime(float w)
{
	controller->setWaitTime(w);
}

void EnemyCharacter::setPatrolling(bool b)
{
	controller->setPatrol(b);
}

void EnemyCharacter::addPatrolTarget(int x, int z)
{
	controller->addPatrolTarget(x, z);
}

void EnemyCharacter::attack()
{
	if (!weapon)
	{
		weapon = new EmittingParticleSystem(device);
		weapon->setPosition(position.x, position.y + 0.5f, position.z);
		weapon->setMaterial(weaponMaterial);
		weapon->setParticleDuration(0.8f);
		weapon->setDirection(XMFLOAT3(0.f, yaw, 0.f));
		weapon->setParticleDensity(2);
		particleSystemManager->addParticleSystem(weapon);
	}
	else
	{
		weapon->setDirection(XMFLOAT3(0.f, yaw, 0.f));
	}
	isAttacking = true;
}

void EnemyCharacter::stopAttacking()
{
	if (weapon)
	{
		weapon->setActive(false);
		weapon = nullptr;
	}
}

void EnemyCharacter::update(float deltaTime)
{
	Character::update(deltaTime);
	if (isAttacking)
	{
		playerCharacter->takeDamage(damage*deltaTime);
	}
	isAttacking = false;
}

