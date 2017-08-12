#pragma once
#include "Shape.h"

class Point :public Shape
{
	struct cbPerObject
	{
		XMMATRIX  WVP;
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
	};

	struct cbPerFrame
	{
		Light  light;
	};

protected:
	Light light;
	cbPerFrame constbuffPerFrame;
	XMFLOAT4 color;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA iinitData;
public:
	Point(ID3D11Device* device);
	virtual ~Point();
	virtual void setMaterial(Material* m) override;
	void setColor(float r, float g, float b, float a);
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection) override;
	void setAmbientLight(float r, float g, float b, float a);
};