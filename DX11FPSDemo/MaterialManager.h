#pragma once
#include <d3d11.h>
#include "Material.h"

class MaterialManager
{
	static ID3D11Device* device;
	static MaterialManager* instance;
	Material* defaultMaterial;
	Material* brickWall;
	Material* stoneFloor;
	Material* stone;
	Material* steel;
	Material* bronze;
	Material* ruby;
	Material* emerald;
	Material* rectangleShape;
	Material* pointParticle;
	MaterialManager();
public:
	static void setDevice(ID3D11Device* d) { device = d; }
	static MaterialManager* getInstance();
	~MaterialManager();
	Material* getDefaultMaterial() const { return defaultMaterial; }
	Material* getBrickWallMaterial() const { return brickWall; }
	Material* getStoneFloor() const { return stoneFloor; }
	Material* getStone() const { return stone; }
	Material* getSteel() const { return steel; }
	Material* getBronze() const { return bronze; }
	Material* getRuby() const { return ruby; }
	Material* getEmerald()const { return emerald; }
	Material* getrectangleShape()const { return rectangleShape; }
	Material* getPointParticle()const { return pointParticle; }
};