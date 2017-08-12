#include "UIComponent.h"

UIComponent::UIComponent(ID3D11Device * d) :device(d),
position(0.f, 0.f, 0.f),
rotation(0.f, 0.f, 0.f),
scaling(1.f, 1.f, 1.f),
transform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
{
}

UIComponent::~UIComponent()
{
}

void UIComponent::setPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void UIComponent::setRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void UIComponent::setScaling(float x, float y, float z)
{
	scaling = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void UIComponent::update(float deltaTime)
{
}

void UIComponent::draw(ID3D11DeviceContext * deviceContext, CXMMATRIX camView, CXMMATRIX camProjection)
{

}
