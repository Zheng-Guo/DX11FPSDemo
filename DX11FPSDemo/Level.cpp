#include "Level.h"

Level::Level(ID3D11Device* d) :device(d),
map(nullptr),
mapWidth(0),
mapHeight(0)
{
}

Level::~Level()
{
	for (WallBlock *w : wallBlocks)
		delete w;
	for (Floor *f : floors)
		delete f;
	for (int i = 0; i < mapHeight; ++i)
		delete[] map[i];
	delete[] map;
}

void Level::setLayout(int ** m, int width, int height)
{
	for (WallBlock *w : wallBlocks)
		delete w;
	wallBlocks.clear();
	for (Floor *f : floors)
		delete f;
	floors.clear();
	for (int i = 0; i < mapHeight; ++i)
		delete[] map[i];
	delete[] map;
	map = m;
	mapWidth = width;
	mapHeight = height;
	int floorColumn = width / 4 + 1, floorRow = height / 4 + 1;
	for (int i = 0; i<floorRow; ++i)
	{
		for (int j = 0; j<floorColumn; ++j)
		{
			Floor *f = new Floor(device);
			f->setScaling(4.f, 1.f, 4.f);
			f->setPosition(j * 8 + 4, -2.f, i * 8 + 4);
			floors.push_back(f);
		}
	}
	for (int i = 0; i<height; ++i)
	{
		for (int j = 0; j<width; ++j)
		{
			if (map[i][j] == 1)
			{
				WallBlock *w = new WallBlock(device);
				w->setPosition(j * 2 + 1, 0.f, i * 2 + 1);
				wallBlocks.push_back(w);
				WallBlock *w2 = new WallBlock(device);
				w2->setPosition(j * 2 + 1, 2.f, i * 2 + 1);
				wallBlocks.push_back(w2);
				WallBlock *w3 = new WallBlock(device);
				w3->setPosition(j * 2 + 1, 4.f, i * 2 + 1);
				wallBlocks.push_back(w3);
			}
			if (map[i][j] == 0)
			{
				navigableGrids.push_back(pair<int, int>(j, i));
			}
		}
	}
}

void Level::setWallMaterial(Material * m)
{
	wallMaterial = m;
	for (WallBlock* w : wallBlocks)
		w->setMaterial(m);
}

void Level::setFloorMaterial(Material * m)
{
	floorMaterial = m;
	for (Floor* f : floors)
		f->setMaterial(m);
}
