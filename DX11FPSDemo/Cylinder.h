#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX  World;
	};

	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		XMFLOAT3 dir;
		float pad;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
	};

	struct cbPerFrame
	{
		Light  light;
	};

protected:
	Light light;
	cbPerFrame constbuffPerFrame;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA iinitData;
public:
	Cylinder(ID3D11Device* device);
	virtual ~Cylinder();
	virtual void setMaterial(Material* m) override;
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection) override;
	virtual void translate(float x, float y, float z) override;
	virtual void rotate(float x, float y, float z) override;
	virtual void scale(float x, float y, float z) override;
	virtual void setPosition(float x, float y, float z) override;
	virtual void setRotation(float x, float y, float z) override;
	virtual void setScale(float x, float y, float z) override;
};