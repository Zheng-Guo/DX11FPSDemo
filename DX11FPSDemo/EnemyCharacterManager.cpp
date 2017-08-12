#include "EnemyCharacterManager.h"

EnemyCharacterManager* EnemyCharacterManager::instance = nullptr;

EnemyCharacterManager::EnemyCharacterManager() :spawnIntervalBase(5),
spawnIntervalMargin(5),
spawnRange(10.f)
{
	spawnInterval = rand() % spawnIntervalMargin + spawnIntervalBase;
}
EnemyCharacterManager * EnemyCharacterManager::getInstance()
{
	if (!instance)
	{
		instance = new EnemyCharacterManager;
	}
	return instance;
}

EnemyCharacterManager::~EnemyCharacterManager()
{
	for (EnemyCharacter* c : enemyCharacters)
		delete c;
}

void EnemyCharacterManager::setSpawnInterval(int base, int margin)
{
	spawnIntervalBase = base;
	spawnIntervalMargin = margin;
}

void EnemyCharacterManager::setAvailableGrids(Level * l)
{
	availableGrids = l->getNavigableGridList();
}

void EnemyCharacterManager::update(float deltaTime)
{
	int i = 0;
	while (i<enemyCharacters.size())
	{
		if (enemyCharacters[i]->isActive())
		{
			enemyCharacters[i]->update(deltaTime);
			++i;
		}
		else
		{
			delete enemyCharacters[i];
			enemyCharacters.erase(enemyCharacters.begin() + i);
		}
	}
	if (spawnInterval>0)
	{
		spawnInterval -= deltaTime;
	}
	else
	{
		spawnInterval = rand() % spawnIntervalMargin + spawnIntervalBase;
		EnemyCharacter* enemy = new EnemyCharacter(device);
		enemy->setMaterial(characterMaterial);
		enemy->setLinearSpeed(5.f);
		enemy->setWaitTime(3.f);
		enemy->setPatrolling(false);
		enemy->setPlayerCharacter(playerCharacter);
		enemy->setFireRange(7.f);
		enemy->setWeaponMaterial(weaponMaterial);
		XMFLOAT3 playerPosition = playerCharacter->getPosition();
		while (true)
		{
			int index = rand() % availableGrids.size();
			int destX = availableGrids[index].first * 2 + 1;
			int destZ = availableGrids[index].second * 2 + 1;
			float distanceSquared = (destX - playerPosition.x)*(destX - playerPosition.x) + (destZ - playerPosition.z)*(destZ - playerPosition.z);
			if (distanceSquared >= spawnRange*spawnRange)
			{
				enemy->setPosition(destX, 1.f, destZ);
				break;
			}
		}
		enemyCharacters.push_back(enemy);
	}
}

void EnemyCharacterManager::reset()
{
	for (EnemyCharacter *c : enemyCharacters)
		delete c;
	enemyCharacters.clear();
}
