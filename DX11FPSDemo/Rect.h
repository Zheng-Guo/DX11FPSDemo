#pragma once
#include "Shape.h"

class Rect :public Shape
{
	struct cbPerObject
	{
		XMMATRIX  WV;
	};

protected:
	XMFLOAT4 color;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA iinitData;
public:
	Rect(ID3D11Device* device);
	~Rect();
	virtual void setMaterial(Material* m) override;
	void setColor(float r, float g, float b, float a);
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection) override;
};