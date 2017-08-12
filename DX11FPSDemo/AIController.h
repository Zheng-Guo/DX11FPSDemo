#pragma once
#include "EnemyCharacter.h"
#include "NavigationManager.h"
using namespace std;

class AIController
{
	EnemyCharacter *controlledCharacter;
	bool active, isMoving, isPatrolling, isWaiting;
	float stopRange;
	float waitTime, waitedTime;
	int patrolPointIndex;
	pair<int, int> destination;
	vector<pair<int, int>> currentPath;
	vector<pair<int, int>> patrolTargets;
	NavigationManager *navigationManager;
	void characterMoveTo(int x, int z);
public:
	AIController();
	void setControlledCharacter(EnemyCharacter *c) { controlledCharacter = c; }
	void setStopRange(float s) { stopRange = s; }
	void setWaitTime(float w) { waitTime = w; }
	void setActive(bool a) { active = a; }
	void setPatrol(bool p) { isPatrolling = p; }
	void addPatrolTarget(int x, int z) { patrolTargets.push_back(pair<int, int>(x, z)); }
	void update(float deltaTime);
	bool isActive()const { return active; }
};