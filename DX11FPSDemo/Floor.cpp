#include "Floor.h"
#include "CollisionDetection.h"
#include "Cube.h"

Floor::Floor(ID3D11Device * d) :GameObject(d)
{
	Cube *c = new Cube(device);
	GameObject::addShape(c);
	GameObject::setCollider(new CubicCollider);
}

void Floor::setMaterial(Material * m)
{
	for (Shape *s : GameObject::shapes)
		s->setMaterial(m);
}

void Floor::setScaling(float x, float y, float z)
{
	GameObject::setScaling(x, y, z);
	CubicCollider *c = (CubicCollider*)collider;
	c->setHalfWidth(x);
	c->setHalfHeight(y);
	c->setHalfThickness(z);
}
