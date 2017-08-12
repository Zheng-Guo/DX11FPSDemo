#pragma once
#include <vector>
#include "Level.h"

using namespace std;

class NavigationManager
{
	int **map;
	int mapWidth, mapHeight;
	vector<pair<int, int>> navigableGrids;
	static NavigationManager* instance;
	NavigationManager();
public:
	static NavigationManager* getInstance();
	void setMap(Level *l);
	vector<pair<int, int>> pathFinding(int startX, int startY, int endX, int endY) const;
	vector<pair<int, int>> getNavigableGridList() const { return navigableGrids; }
};