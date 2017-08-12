#pragma once
#include <vector>
#include "WallBlock.h"
#include "Floor.h"
#include "Material.h"

using namespace std;

class Level
{
	ID3D11Device* device;
	vector<WallBlock*> wallBlocks;
	vector<Floor*> floors;
	vector<pair<int, int>> navigableGrids;
	int **map;
	int mapWidth, mapHeight;
	Material *wallMaterial, *floorMaterial;
public:
	Level(ID3D11Device* d);
	~Level();
	void setLayout(int **m, int width, int height);
	void setWallMaterial(Material *m);
	void setFloorMaterial(Material *m);
	vector<WallBlock*>::const_iterator getWallBlockListBegin() { return wallBlocks.begin(); }
	vector<WallBlock*>::const_iterator getWallBlockListEnd() { return wallBlocks.end(); }
	vector<Floor*>::const_iterator getFloorListBegin() { return floors.begin(); }
	vector<Floor*>::const_iterator getFloorListEnd() { return floors.end(); }
	int** getMap()const { return map; }
	int getMapWidth() const { return mapWidth; }
	int getMapHeight()const { return mapHeight; }
	vector<pair<int, int>>::const_iterator getNavigableGridListBegin() { return navigableGrids.begin(); }
	vector<pair<int, int>>::const_iterator getNavigableGridListEnd() { return navigableGrids.end(); }
	vector<pair<int, int>> getNavigableGridList() const { return navigableGrids; }
};