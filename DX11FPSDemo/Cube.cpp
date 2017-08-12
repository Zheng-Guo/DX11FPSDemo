#include "Cube.h"
#include "GeometryManager.h"

Cube::Cube(ID3D11Device* device) :Shape(device),
cornerA(-1.f, -1.f, -1.f),
cornerB(1.f, 1.f, 1.f)
{
	type = ShapeType::Cube;
	GeometryManager* geoManager = GeometryManager::getInstance();
	numberOfIndices = geoManager->getCubeIndexCount();
	vertexSize = geoManager->getVertexSize();


	light.dir = XMFLOAT3(-1.f, 1.f, 0.5f);
	light.ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	light.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = geoManager->getVertexSize() * geoManager->getCubeVertexCount();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = geoManager->getCubeVertitces();
	md3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * geoManager->getCubeIndexCount();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	iinitData.pSysMem = geoManager->getCubeIndices();
	md3dDevice->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	md3dDevice->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	md3dDevice->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);


	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	md3dDevice->CreateRasterizerState(&wfdesc, &WireFrame);

}

Cube::~Cube()
{
}

void Cube::setMaterial(Material * m)
{
	Shape::setMaterial(m);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	md3dDevice->CreateInputLayout(layout, numElements, material->getVertexShaderBuffer()->GetBufferPointer(), material->getVertexShaderBuffer()->GetBufferSize(), &vertLayout);

	CubesTexture = material->getTexture();

	CubesTexSamplerState = material->getTextureSampleState();
}

void Cube::draw(ID3D11DeviceContext * deviceContext, CXMMATRIX camView, CXMMATRIX camProjection)
{
	deviceContext->VSSetShader(material->getVertexShader(), nullptr, 0);
	deviceContext->PSSetShader(material->getPixelShader(), nullptr, 0);
	deviceContext->PSSetShaderResources(0, 1, &CubesTexture);
	deviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
	XMMATRIX world = XMLoadFloat4x4(&transform)*XMLoadFloat4x4(&parentTransform);
	XMMATRIX wvp = world*camView*camProjection;

	UINT stride = vertexSize;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetInputLayout(vertLayout);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(WireFrame);

	cbPerObject cbPerObj;
	cbPerObj.World = XMMatrixTranspose(world);
	cbPerObj.WVP = XMMatrixTranspose(world*camView*camProjection);
	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	constbuffPerFrame.light = light;
	deviceContext->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);

	deviceContext->DrawIndexed(numberOfIndices, 0, 0);
}

void Cube::translate(float x, float y, float z)
{
	Shape::translate(x, y, z);
}

void Cube::rotate(float x, float y, float z)
{
	Shape::rotate(x, y, z);
}

void Cube::scale(float x, float y, float z)
{
	Shape::scale(x, y, z);
}

void Cube::setPosition(float x, float y, float z)
{
	Shape::setPosition(x, y, z);
}

void Cube::setRotation(float x, float y, float z)
{
	Shape::setRotation(x, y, z);
}

void Cube::setScale(float x, float y, float z)
{
	Shape::setScale(x, y, z);
}
