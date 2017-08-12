#pragma once
#include "Shape.h"

class SkyBox :public Shape
{
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX  World;
	};

	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z)
			: pos(x, y, z) {}

		XMFLOAT3 pos;
	};
protected:
	cbPerObject constbuffPerObj;
	const int numberOfLongitudes;
	const int numberOfLatitudes;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA iinitData;
public:
	SkyBox(ID3D11Device* device);
	virtual ~SkyBox();
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection) override;
	virtual void translate(float x, float y, float z) override;
	virtual void rotate(float x, float y, float z) override;
	virtual void scale(float x, float y, float z) override;
	virtual void setPosition(float x, float y, float z) override;
	virtual void setRotation(float x, float y, float z) override;
	virtual void setScale(float x, float y, float z) override;
};