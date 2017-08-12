#include "Projectile.h"

Projectile::Projectile(ID3D11Device * d) :GameObject(d),
velocity(0.f, 0.f, 0.f),
damage(0),
duration(0),
active(false)
{
}

void Projectile::setVelocity(XMFLOAT3 v)
{
	velocity = v;
	collider->setVelocity(v);
}

void Projectile::update(float deltaTime)
{
	duration -= deltaTime;
	if (duration <= 0)
	{
		active = false;
		return;
	}
	XMVECTOR v = XMLoadFloat3(&velocity);
	XMVECTOR newPos = XMLoadFloat3(&position) + v*deltaTime;
	XMFLOAT3 np;
	XMStoreFloat3(&np, newPos);
	GameObject::setPosition(np.x, np.y, np.z);
}
