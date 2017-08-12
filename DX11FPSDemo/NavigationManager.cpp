#include <algorithm>
#include <stack>
#include "NavigationManager.h"

using namespace std;

NavigationManager* NavigationManager::instance = nullptr;

NavigationManager::NavigationManager() :map(nullptr),
mapWidth(0),
mapHeight(0)
{
}

NavigationManager * NavigationManager::getInstance()
{
	if (!instance)
	{
		instance = new NavigationManager;
	}
	return instance;
}

void NavigationManager::setMap(Level * l)
{
	map = l->getMap();
	mapWidth = l->getMapWidth();
	mapHeight = l->getMapHeight();
	navigableGrids = l->getNavigableGridList();
}

struct Node
{
	int x, y;
	int gValue, hValue;
	Node *parent;
	Node(int a, int b) :x(a), y(b), gValue(0), hValue(0), parent(nullptr)
	{
	}
};

vector<pair<int, int>> NavigationManager::pathFinding(int startX, int startY, int endX, int endY) const
{
	vector<pair<int, int>> path;
	if (startX == endX&&startY == endY)
		return path;

	Node ***nodeMap = new Node**[mapHeight];
	for (int i = 0; i<mapHeight; ++i)
	{
		nodeMap[i] = new Node*[mapWidth];
		for (int j = 0; j<mapWidth; ++j)
		{
			if (map[i][j] != 1) {
				nodeMap[i][j] = new Node(j, i);
				nodeMap[i][j]->hValue = (abs(endX - j) + abs(endY - i)) * 10;
			}
			else
				nodeMap[i][j] = nullptr;
		}
	}

	auto comp = [](Node *a, Node *b) {return (a->gValue + a->hValue) > (b->gValue + a->hValue); };
	vector<Node*> openList;
	vector<Node*> v;
	int currentX, currentY, currentGValue;
	Node *currentParent = nullptr;
	do
	{
		if (openList.empty())
		{
			currentX = startX;
			currentY = startY;
			currentGValue = 0;
		}
		else
		{
			currentParent = openList[0];
			currentX = currentParent->x;
			currentY = currentParent->y;
			currentGValue = currentParent->gValue;
			v.push_back(currentParent);
			openList.erase(openList.begin());
		}
		if (currentX == endX&&currentY == endY)
			break;

		if (currentX<mapWidth - 1 && map[currentY][currentX + 1] != 1)
		{
			if (nodeMap[currentY][currentX + 1]->gValue == 0)
			{
				nodeMap[currentY][currentX + 1]->gValue = currentGValue + 10;
				nodeMap[currentY][currentX + 1]->parent = currentParent;
				openList.push_back(nodeMap[currentY][currentX + 1]);
			}
			else if (currentGValue + 10<nodeMap[currentY][currentX + 1]->gValue)
			{
				nodeMap[currentY][currentX + 1]->gValue = currentGValue + 10;
				nodeMap[currentY][currentX + 1]->parent = currentParent;
			}
		}

		if (currentX<mapWidth - 1 && map[currentY][currentX + 1] != 1 && currentY<mapHeight - 1 && map[currentY + 1][currentX] != 1 && map[currentY + 1][currentX + 1] != 1)
		{
			if (nodeMap[currentY + 1][currentX + 1]->gValue == 0)
			{
				nodeMap[currentY + 1][currentX + 1]->gValue = currentGValue + 14;
				nodeMap[currentY + 1][currentX + 1]->parent = currentParent;
				openList.push_back(nodeMap[currentY + 1][currentX + 1]);
			}
			else if (currentGValue + 14<nodeMap[currentY + 1][currentX + 1]->gValue)
			{
				nodeMap[currentY + 1][currentX + 1]->gValue = currentGValue + 14;
				nodeMap[currentY + 1][currentX + 1]->parent = currentParent;
			}
		}

		if (currentY<mapHeight - 1 && map[currentY + 1][currentX] != 1)
		{
			if (nodeMap[currentY + 1][currentX]->gValue == 0)
			{
				nodeMap[currentY + 1][currentX]->gValue = currentGValue + 10;
				nodeMap[currentY + 1][currentX]->parent = currentParent;
				openList.push_back(nodeMap[currentY + 1][currentX]);
			}
			else if (currentGValue + 10<nodeMap[currentY + 1][currentX]->gValue)
			{
				nodeMap[currentY + 1][currentX]->gValue = currentGValue + 10;
				nodeMap[currentY + 1][currentX]->parent = currentParent;
			}
		}

		if (currentX>0 && map[currentY][currentX - 1] != 1 && currentY<mapHeight - 1 && map[currentY + 1][currentX] != 1 && map[currentY + 1][currentX - 1] != 1)
		{
			if (nodeMap[currentY + 1][currentX - 1]->gValue == 0)
			{
				nodeMap[currentY + 1][currentX - 1]->gValue = currentGValue + 14;
				nodeMap[currentY + 1][currentX - 1]->parent = currentParent;
				openList.push_back(nodeMap[currentY + 1][currentX - 1]);
			}
			else if (currentGValue + 14<nodeMap[currentY + 1][currentX - 1]->gValue)
			{
				nodeMap[currentY + 1][currentX - 1]->gValue = currentGValue + 14;
				nodeMap[currentY + 1][currentX - 1]->parent = currentParent;
			}
		}

		if (currentX>0 && map[currentY][currentX - 1] != 1)
		{
			if (nodeMap[currentY][currentX - 1]->gValue == 0)
			{
				nodeMap[currentY][currentX - 1]->gValue = currentGValue + 10;
				nodeMap[currentY][currentX - 1]->parent = currentParent;
				openList.push_back(nodeMap[currentY][currentX - 1]);
			}
			else if (currentGValue + 10<nodeMap[currentY][currentX - 1]->gValue)
			{
				nodeMap[currentY][currentX - 1]->gValue = currentGValue + 10;
				nodeMap[currentY][currentX - 1]->parent = currentParent;
			}
		}

		if (currentX>0 && map[currentY][currentX - 1] != 1 && currentY>0 && map[currentY - 1][currentX] != 1 && map[currentY - 1][currentX - 1] != 1)
		{
			if (nodeMap[currentY - 1][currentX - 1]->gValue == 0)
			{
				nodeMap[currentY - 1][currentX - 1]->gValue = currentGValue + 14;
				nodeMap[currentY - 1][currentX - 1]->parent = currentParent;
				openList.push_back(nodeMap[currentY - 1][currentX - 1]);
			}
			else if (currentGValue + 14<nodeMap[currentY - 1][currentX - 1]->gValue)
			{
				nodeMap[currentY - 1][currentX - 1]->gValue = currentGValue + 14;
				nodeMap[currentY - 1][currentX - 1]->parent = currentParent;
			}
		}

		if (currentY>0 && map[currentY - 1][currentX] != 1)
		{
			if (nodeMap[currentY - 1][currentX]->gValue == 0)
			{
				nodeMap[currentY - 1][currentX]->gValue = currentGValue + 10;
				nodeMap[currentY - 1][currentX]->parent = currentParent;
				openList.push_back(nodeMap[currentY - 1][currentX]);
			}
			else if (currentGValue + 10<nodeMap[currentY - 1][currentX]->gValue)
			{
				nodeMap[currentY - 1][currentX]->gValue = currentGValue + 10;
				nodeMap[currentY - 1][currentX]->parent = currentParent;
			}
		}

		if (currentX<mapWidth - 1 && map[currentY][currentX + 1] != 1 && currentY>0 && map[currentY - 1][currentX] != 1 && map[currentY - 1][currentX + 1] != 1)
		{
			if (nodeMap[currentY - 1][currentX + 1]->gValue == 0)
			{
				nodeMap[currentY - 1][currentX + 1]->gValue = currentGValue + 14;
				nodeMap[currentY - 1][currentX + 1]->parent = currentParent;
				openList.push_back(nodeMap[currentY - 1][currentX + 1]);
			}
			else if (currentGValue + 14<nodeMap[currentY - 1][currentX + 1]->gValue)
			{
				nodeMap[currentY - 1][currentX + 1]->gValue = currentGValue + 14;
				nodeMap[currentY - 1][currentX + 1]->parent = currentParent;
			}
		}
		make_heap(openList.begin(), openList.end(), comp);
	} while (!openList.empty());

	if (v.size() == 0 || v[v.size() - 1]->x != endX || v[v.size() - 1]->y != endY)
		return path;

	stack<pair<int, int>> s;
	if (!v.empty())
	{
		Node *destination = v[v.size() - 1];
		while (destination)
		{
			s.push(pair<int, int>(destination->x, destination->y));
			destination = destination->parent;
		}
	}
	while (!s.empty())
	{
		path.push_back(s.top());
		s.pop();
	}
	for (int i = 0; i<mapHeight; ++i)
	{
		for (int j = 0; j<mapWidth; ++j)
		{
			delete nodeMap[i][j];
		}
		delete[] nodeMap[i];
	}
	delete[] nodeMap;

	return path;
}
