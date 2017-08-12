#pragma once
#include <vector>
#include "AIController.h"

class AIManager
{
	vector<AIController*> controllers;
	static AIManager* instance;
	AIManager();
public:
	static AIManager* getInstance();
	~AIManager();
	void addController(AIController *c);
	void update(float deltaTime);
	void reset();
};