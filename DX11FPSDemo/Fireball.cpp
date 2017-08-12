#include "Fireball.h"
#include "Sphere.h"
#include "CollisionDetection.h"

Fireball::Fireball(ID3D11Device * d, float r) :Projectile(d),
radius(r)
{
	Sphere *s = new Sphere(d);
	s->setScale(r, r, r);
	GameObject::addShape(s);
	GameObject::setCollider(new SphericalCollider(r));
}

void Fireball::setMaterial(Material * m)
{
	for (Shape *s : GameObject::shapes)
		s->setMaterial(m);
}

void Fireball::update(float deltaTime)
{
	Projectile::update(deltaTime);
}
