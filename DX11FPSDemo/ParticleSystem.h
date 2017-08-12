#pragma once
#include <vector>
#include "Particle.h"
using namespace std;

class ParticleSystem
{
protected:
	vector<Particle*> particles;
	ID3D11Device* device;
	XMFLOAT3 position;
	XMFLOAT4X4 parentTransform;
	bool active;
	Material *material;
	XMFLOAT4 color;
public:
	ParticleSystem(ID3D11Device* d);
	virtual ~ParticleSystem();
	void setPosition(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	void setMaterial(Material *m);
	void setColor(XMFLOAT4 color);
	void setParentTransform(XMFLOAT4X4 t) { parentTransform = t; }
	void setActive(bool b) { active = b; }
	bool isActive() const { return active; }
	vector<Particle*>::iterator getListBegin() { return particles.begin(); }
	vector<Particle*>::iterator getListEnd() { return particles.end(); }
	virtual void update(float deltaTime);
};