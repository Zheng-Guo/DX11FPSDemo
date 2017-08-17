#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "Material.h"

class Shape
{
public:
	enum ShapeType { Cube, Cylinder, Sphere, Point, Rectangle };
protected:
	ShapeType type;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scaling;
	XMFLOAT4X4 transform;
	XMFLOAT4X4 parentTransform;
	XMFLOAT3 animationPivot;
	XMFLOAT4X4 animationTransform;
	int numberOfIndices;
	int vertexSize;

	ID3D11Device* md3dDevice;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11InputLayout* vertLayout;
	ID3D11RasterizerState* WireFrame;

	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11Buffer* cbPerFrameBuffer;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;

	Material* material;
public:
	Shape(ID3D11Device* device);
	virtual ~Shape();
	ShapeType getShapeType() const { return type; }
	virtual void setMaterial(Material* m) { material = m; }
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection) = 0;
	virtual void translate(float x, float y, float z);
	virtual void rotate(float x, float y, float z);
	virtual void scale(float x, float y, float z);
	virtual void setPosition(float x, float y, float z);
	virtual void setRotation(float x, float y, float z);
	virtual void setScale(float x, float y, float z);
	XMFLOAT3 getScale()const { return scaling; }
	void setParentTransform(XMFLOAT4X4 p) { parentTransform = p; }
	void setAnimationPivot(float x, float y, float z) { animationPivot = XMFLOAT3(x, y, z); }
	void setAnimationTransform(XMFLOAT4X4 t) { animationTransform = t; }
};