#include "Rect.h"

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float cr, float cg, float cb, float ca)
		: pos(x, y, z), color(cr, cg, cb, ca) {}

	XMFLOAT3 pos;
	XMFLOAT4 color;
};

Rect::Rect(ID3D11Device * device) :Shape(device)
{
	type = ShapeType::Rectangle;
	numberOfIndices = 6;
	vertexSize = sizeof(Vertex);

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	md3dDevice->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	md3dDevice->CreateRasterizerState(&wfdesc, &WireFrame);
}

Rect::~Rect()
{
}

void Rect::setMaterial(Material * m)
{
	Shape::setMaterial(m);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	md3dDevice->CreateInputLayout(layout, numElements, material->getVertexShaderBuffer()->GetBufferPointer(), material->getVertexShaderBuffer()->GetBufferSize(), &vertLayout);
}

void Rect::setColor(float r, float g, float b, float a)
{
	Vertex v[] = { Vertex(-1.f,-1.f,-1.f,r,g,b,a),
		Vertex(-1.f,1.f,-1.f,r,g,b,a),
		Vertex(1.f,1.f,-1.f,r,g,b,a) ,
		Vertex(1.f,-1.f,-1.f,r,g,b,a) };
	UINT index[] = { 0,1,2,
		0,2,3 };

	vertexBufferData.pSysMem = v;
	md3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	iinitData.pSysMem = index;
	md3dDevice->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);
}

void Rect::draw(ID3D11DeviceContext * deviceContext, CXMMATRIX camView, CXMMATRIX camProjection)
{
	deviceContext->VSSetShader(material->getVertexShader(), nullptr, 0);
	deviceContext->PSSetShader(material->getPixelShader(), nullptr, 0);
	XMMATRIX world = XMLoadFloat4x4(&transform)*XMLoadFloat4x4(&parentTransform);

	UINT stride = vertexSize;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetInputLayout(vertLayout);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(WireFrame);

	cbPerObject cbPerObj;
	cbPerObj.WV = XMMatrixTranspose(world);
	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	deviceContext->DrawIndexed(numberOfIndices, 0, 0);
}
