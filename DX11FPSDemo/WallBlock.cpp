#include "WallBlock.h"
#include "Cube.h"
#include "CollisionDetection.h"

WallBlock::WallBlock(ID3D11Device* d) :GameObject(d)
{
	Cube *c = new Cube(device);
	GameObject::addShape(c);
	GameObject::setCollider(new CubicCollider);
}

void WallBlock::setMaterial(Material * m)
{
	for (Shape *s : GameObject::shapes)
		s->setMaterial(m);
}

void WallBlock::setScaling(float x, float y, float z)
{
	GameObject::setScaling(x, y, z);
	CubicCollider *c = (CubicCollider*)collider;
	c->setHalfWidth(x);
	c->setHalfHeight(y);
	c->setHalfThickness(z);
}
