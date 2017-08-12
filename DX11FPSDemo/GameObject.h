#pragma once
#include <vector>
#include "Shape.h"
#include "Collider.h"

using namespace std;

class GameObject
{
protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scaling;
	XMFLOAT4X4 transform;

	ID3D11Device* device;

	vector<Shape*> shapes;

	Collider *collider;
public:
	GameObject(ID3D11Device* d);
	virtual ~GameObject();
	void setCollider(Collider *c);
	void setPosition(float x, float y, float z);
	virtual void setRotation(float x, float y, float z);
	virtual void setScaling(float x, float y, float z);
	void move(XMFLOAT3 offset);
	XMFLOAT3 getPosition()const { return position; }
	XMFLOAT3 getRotation()const { return rotation; }
	XMFLOAT3 getScaling() const { return scaling; }
	XMFLOAT4X4 getTransform() const { return transform; }
	Collider* getCollider() const { return collider; }
	void addShape(Shape* s);
	virtual void update(float deltaTime);
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection);
};