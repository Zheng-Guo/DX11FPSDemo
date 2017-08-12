#pragma once
#include <vector>
#include "Fireball.h"
#include "Material.h"

using namespace std;

class ProjectileManager
{
	vector<Fireball*> projectiles;
	Material *material;
	static ProjectileManager *instance;
public:
	~ProjectileManager();
	static ProjectileManager* getInstance();
	void setMaterial(Material *m) { material = m; }
	void addProjectile(Fireball *f);
	void update(float deltaTime);
	vector<Fireball*>::const_iterator getListBegin() const { return projectiles.begin(); }
	vector<Fireball*>::const_iterator getListEnd() const { return projectiles.end(); }
	void reset();
};