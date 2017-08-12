#pragma once
#include <d3d11.h>
#include <xnamath.h>

class UIComponent
{
protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scaling;
	XMFLOAT4X4 transform;

	ID3D11Device* device;
public:
	UIComponent(ID3D11Device* d);
	virtual ~UIComponent();
	virtual void setPosition(float x, float y, float z);
	virtual void setRotation(float x, float y, float z);
	virtual void setScaling(float x, float y, float z);
	XMFLOAT3 getPosition()const { return position; }
	XMFLOAT3 getRotation()const { return rotation; }
	XMFLOAT3 getScaling() const { return scaling; }
	XMFLOAT4X4 getTransform() const { return transform; }
	virtual void update(float deltaTime);
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection);
};