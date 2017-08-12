#include <fstream>
#include "LevelLoader.h"
using namespace std;

LevelLoader::LevelLoader(ID3D11Device * d) :device(d)
{
}

Level * LevelLoader::loadLevelFromFile(const char * filename)
{
	ifstream ifs(filename);
	int width, height;
	int **map;
	ifs >> width >> height;
	map = new int*[height];
	for (int i = 0; i<height; ++i)
	{
		map[i] = new int[width];
		for (int j = 0; j<width; ++j)
		{
			ifs >> map[i][j];
		}
	}
	ifs.close();

	Level *level = new Level(device);
	level->setLayout(map, width, height);

	return level;
}
