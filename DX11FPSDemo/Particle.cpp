#include "Particle.h"
#include "Point.h"
#include "Sphere.h"

Particle::Particle(ID3D11Device* device) :GameObject(device),
velocity(0.f, 0.f, 0.f),
acceleration(0.f, 0.f, 0.f),
duration(0),
color(1.f, 1.f, 1.f, 1.f)
{
	Sphere *s = new Sphere(device);
	GameObject::addShape(s);
}

void Particle::setVelocity(float x, float y, float z)
{
	velocity = XMFLOAT3(x, y, z);
}

void Particle::setAcceleration(float x, float y, float z)
{
	acceleration = XMFLOAT3(x, y, z);
}

void Particle::setColor(float r, float g, float b, float a)
{
	color = XMFLOAT4(r, g, b, a);
	for (Shape *s : GameObject::shapes)
	{
		Point *p = (Point*)s;
		p->setColor(r, g, b, a);
	}
}

void Particle::setMaterial(Material * m)
{
	for (Shape *s : GameObject::shapes)
		s->setMaterial(m);
}

void Particle::setAmbientLight(float r, float g, float b, float a)
{
	for (Shape *s : GameObject::shapes)
	{
		Point *p = (Point*)s;
		p->setAmbientLight(r, g, b, a);
	}
}

void Particle::update(float deltaTime)
{
	XMVECTOR displacement = XMLoadFloat3(&acceleration)*deltaTime*deltaTime*0.5f + XMLoadFloat3(&velocity)*deltaTime;
	XMVECTOR newVelocity = XMLoadFloat3(&velocity) + XMLoadFloat3(&acceleration)*deltaTime;
	XMVECTOR newPosition = XMLoadFloat3(&position) + displacement;
	XMStoreFloat3(&velocity, newVelocity);
	setPosition(XMVectorGetX(newPosition), XMVectorGetY(newPosition), XMVectorGetZ(newPosition));
	duration -= deltaTime;
}
