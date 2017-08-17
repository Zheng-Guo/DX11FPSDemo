#include "Shape.h"

Shape::Shape(ID3D11Device* device) :position(0.f, 0.f, 0.f),
rotation(0.f, 0.f, 0.f),
scaling(1.f, 1.f, 1.f),
transform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
parentTransform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
animationPivot(0.f,0.f,0.f),
animationTransform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
numberOfIndices(0),
vertexSize(0)
{
	md3dDevice = device;
}

Shape::~Shape()
{
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
	if (vertLayout) vertLayout->Release();
	if (WireFrame) WireFrame->Release();
	if (cbPerObjectBuffer) cbPerObjectBuffer->Release();
	if (cbPerFrameBuffer) cbPerFrameBuffer->Release();
	if (VS) VS->Release();
	if (PS) PS->Release();
	if (VS_Buffer) VS_Buffer->Release();
	if (PS_Buffer) PS_Buffer->Release();
}

void Shape::translate(float x, float y, float z)
{
	position = XMFLOAT3(position.x + x, position.y + y, position.z + z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void Shape::rotate(float x, float y, float z)
{
	rotation = XMFLOAT3(rotation.x + x, rotation.y + y, rotation.z + z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void Shape::scale(float x, float y, float z)
{
	scaling = XMFLOAT3(scaling.x + x, scaling.y + y, scaling.z + z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void Shape::setPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void Shape::setRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}

void Shape::setScale(float x, float y, float z)
{
	scaling = XMFLOAT3(x, y, z);
	XMMATRIX newTransform = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixRotationX(rotation.x)*XMMatrixRotationY(rotation.y)*XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&transform, newTransform);
}