#pragma once
#include <vector>
#include <cstdlib>
#include "EnemyCharacter.h"
#include "Level.h"

using namespace std;

class EnemyCharacterManager
{
	ID3D11Device * device;
	vector<EnemyCharacter*> enemyCharacters;
	static EnemyCharacterManager* instance;
	int spawnIntervalBase, spawnIntervalMargin;
	float spawnInterval;
	Material *characterMaterial;
	Material *weaponMaterial;
	PlayerCharacter *playerCharacter;
	vector<pair<int, int>> availableGrids;
	float spawnRange;
	EnemyCharacterManager();
public:
	static EnemyCharacterManager* getInstance();
	void setDevice(ID3D11Device * d) { device = d; }
	~EnemyCharacterManager();
	void setSpawnInterval(int base, int margin);
	void setCharacterMaterial(Material *m) { characterMaterial = m; }
	void setWeaponMaterial(Material *m) { weaponMaterial = m; }
	void setPlayerCharacter(PlayerCharacter *p) { playerCharacter = p; }
	void setAvailableGrids(Level *l);
	void setSpawnRange(float s) { spawnRange = s; }
	vector<EnemyCharacter*>::iterator getListBegin() { return enemyCharacters.begin(); }
	vector<EnemyCharacter*>::iterator getListEnd() { return enemyCharacters.end(); }
	void update(float deltaTime);
	void reset();
};