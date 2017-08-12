#include "PlayerCharacter.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"

PlayerCharacter::PlayerCharacter(ID3D11Device * d) :FirstPersonCharacter(d),
life(100.f),
totalLife(100.f),
mana(100.f),
totalMana(100.f),
manaRecoveryRate(1.f),
score(0),
fireballSpeed(0),
fireballManaCost(20.f),
projectileOffset(XMFLOAT3(0.f, 0.f, 0.f)),
fire(false)
{
	setCameraOffset(0.f, 1.8f, 1.f);

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

	projectileManager = ProjectileManager::getInstance();
}

void PlayerCharacter::setMaterial(Material * m)
{
	for (Shape *s : GameObject::shapes)
		s->setMaterial(m);
}

void PlayerCharacter::takeDamage(float d)
{
	if (life > 0)
		life -= d;
	else
		life = 0;
}

void PlayerCharacter::fireFireball()
{
	if (mana >= fireballManaCost)
	{
		Fireball *fireball = new Fireball(device, 0.1f);
		fireball->setDamage(2.f);
		XMMATRIX camRotation = XMMatrixRotationRollPitchYaw(cameraPitch, yaw, 0);
		XMFLOAT3 camPosition = camera.getPosition(), camFacingDirection = camera.getFacingDirection();
		XMVECTOR initialPosition = XMLoadFloat3(&camPosition) + XMVector3TransformCoord(XMLoadFloat3(&projectileOffset), camRotation);
		XMVECTOR initialVelocity = XMLoadFloat3(&camFacingDirection)*fireballSpeed;
		XMFLOAT3 ip, iv;
		XMStoreFloat3(&ip, initialPosition);
		XMStoreFloat3(&iv, initialVelocity);
		fireball->setPosition(ip.x, ip.y, ip.z);
		fireball->setDuration(5.f);
		fireball->setVelocity(iv);
		fireball->setActive(true);
		projectileManager->addProjectile(fireball);
		mana -= fireballManaCost;
	}
}

void PlayerCharacter::resetOrientation()
{
	yaw = 0;
	cameraPitch = 0;
	rotation = XMFLOAT3(0.f, 0.f, 0.f);
	XMMATRIX camRotation = XMMatrixRotationRollPitchYaw(cameraPitch, yaw, 0);
	XMVECTOR newCameraOffset = XMVector3TransformCoord(XMLoadFloat3(&cameraOffset), camRotation);
	XMStoreFloat3(&currentCameraOffset, newCameraOffset);
	camera.setPitch(cameraPitch);
	camera.setYaw(yaw);
}

void PlayerCharacter::update(float deltaTime)
{
	FirstPersonCharacter::update(deltaTime);
	if (mana<totalMana)
	{
		mana += manaRecoveryRate*deltaTime;
	}
	else
	{
		mana = totalMana;
	}
}
