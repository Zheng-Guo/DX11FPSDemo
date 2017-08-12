#pragma once
#include "Level.h"

class LevelLoader
{
	ID3D11Device* device;
public:
	LevelLoader(ID3D11Device* d);
	Level* loadLevelFromFile(const char* filename);
};