#include "MaterialManager.h"

MaterialManager* MaterialManager::instance = nullptr;
ID3D11Device* MaterialManager::device = nullptr;

MaterialManager::MaterialManager()
{

}

MaterialManager * MaterialManager::getInstance()
{
	if (device)
	{
		if (!instance)
		{
			instance = new MaterialManager();
			instance->defaultMaterial = new Material(device);
			instance->brickWall = new Material(device, L"DefaultEffectFile.fx", L"brick_wall.jpg");
			instance->stoneFloor = new Material(device, L"DefaultEffectFile.fx", L"floor.jpg");
			instance->stone = new Material(device, L"DefaultEffectFile.fx", L"stone.jpg");
			instance->steel = new Material(device, L"DefaultEffectFile.fx", L"steel.jpg");
			instance->bronze = new Material(device, L"DefaultEffectFile.fx", L"bronze.jpg");
			instance->ruby = new Material(device, L"DefaultEffectFile.fx", L"ruby.jpg");
			instance->emerald = new Material(device, L"DefaultEffectFile.fx", L"emerald.jpg");
			instance->rectangleShape = new Material(device, L"RectangleEffectFile.fx");
			instance->pointParticle = new Material(device, L"PointEffectFile.fx");
		}
		return instance;
	}
	return nullptr;
}

MaterialManager::~MaterialManager()
{
	delete defaultMaterial;
	delete brickWall;
	delete stoneFloor;
	delete stone;
	delete steel;
	delete bronze;
	delete ruby;
	delete emerald;
	delete rectangleShape;
	delete pointParticle;
}
