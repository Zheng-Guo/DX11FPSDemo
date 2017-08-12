#include "SkyBox.h"

SkyBox::SkyBox(ID3D11Device* device) :Shape(device),
numberOfLongitudes(60),
numberOfLatitudes(30)
{
	D3DX11CompileFromFile(L"SkyBoxEffects.fx", 0, 0, "VS", "vs_5_0", 0, 0, 0, &VS_Buffer, 0, 0);
	D3DX11CompileFromFile(L"SkyBoxEffects.fx", 0, 0, "PS", "ps_5_0", 0, 0, 0, &PS_Buffer, 0, 0);
	md3dDevice->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	md3dDevice->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	Vertex *v = new Vertex[(numberOfLongitudes + 1)*(numberOfLatitudes + 1)];
	float longitudeAngle = 6.28f / numberOfLongitudes, latitudeAngle = 3.14f / numberOfLatitudes;
	float uInterval = 1.f / numberOfLongitudes, vInterval = 1.f / numberOfLatitudes;
	int number = 0;
	for (int i = 1; i<numberOfLatitudes; ++i)
	{
		float yCoordinate = cos(latitudeAngle*i);
		for (int j = 0; j<numberOfLongitudes; ++j)
		{
			v[i*(numberOfLongitudes + 1) + j] = Vertex(sin(latitudeAngle*i)*cos(longitudeAngle*j), yCoordinate, sin(latitudeAngle*i)*sin(longitudeAngle*j));
			++number;
		}
		v[i*(numberOfLongitudes + 1) + numberOfLongitudes] = Vertex(sin(latitudeAngle*i), yCoordinate, 0);
		++number;
	}
	float edgeUOffset = uInterval / 2;
	for (int i = 0; i<numberOfLongitudes; ++i)
	{
		v[i] = Vertex(0.f, 1.f, 0.f);
		v[i + (numberOfLongitudes + 1)*numberOfLatitudes] = Vertex(0.f, -1.f, 0.f);
		number += 2;
	}
	UINT *indices = new UINT[(numberOfLatitudes - 1)*numberOfLongitudes * 6];
	for (int i = 0; i<numberOfLatitudes; ++i)
	{
		for (int j = 0; j<numberOfLongitudes; ++j)
		{
			if (i == 0)
			{
				indices[j * 3] = j;
				indices[j * 3 + 1] = numberOfLongitudes + j + 2;
				indices[j * 3 + 2] = numberOfLongitudes + j + 1;
			}
			else if (i == numberOfLatitudes - 1)
			{
				indices[numberOfLongitudes * 3 + (numberOfLatitudes - 2)*numberOfLongitudes * 6 + j * 3] = (numberOfLatitudes - 1)*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + (numberOfLatitudes - 2)*numberOfLongitudes * 6 + j * 3 + 1] = (numberOfLatitudes - 1)*(numberOfLongitudes + 1) + j + 1;
				indices[numberOfLongitudes * 3 + (numberOfLatitudes - 2)*numberOfLongitudes * 6 + j * 3 + 2] = numberOfLatitudes*(numberOfLongitudes + 1) + j;
			}
			else
			{
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6] = i*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 1] = i*(numberOfLongitudes + 1) + j + 1;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 2] = (i + 1)*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 3] = (i + 1)*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 4] = i*(numberOfLongitudes + 1) + j + 1;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 5] = (i + 1)*(numberOfLongitudes + 1) + j + 1;
			}
		}
	}

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * (numberOfLongitudes + 1)*(numberOfLatitudes + 1);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	md3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * (numberOfLatitudes - 1)*numberOfLongitudes * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	iinitData.pSysMem = indices;
	md3dDevice->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	md3dDevice->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"grasscube1024.dds", NULL, NULL, &CubesTexture, NULL);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	md3dDevice->CreateSamplerState(&sampDesc, &CubesTexSamplerState);

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

	delete[] v;
	delete[] indices;
}

SkyBox::~SkyBox()
{

}

void SkyBox::draw(ID3D11DeviceContext * deviceContext, CXMMATRIX camView, CXMMATRIX camProjection)
{
	deviceContext->VSSetShader(VS, nullptr, 0);
	deviceContext->PSSetShader(PS, nullptr, 0);
	deviceContext->PSSetShaderResources(0, 1, &CubesTexture);
	deviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
	XMMATRIX world = XMLoadFloat4x4(&transform);
	XMMATRIX wvp = world*camView*camProjection;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetInputLayout(vertLayout);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(WireFrame);

	cbPerObject cbPerObj;
	cbPerObj.WVP = XMMatrixTranspose(world*camView*camProjection);
	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	deviceContext->DrawIndexed((numberOfLatitudes - 1)*numberOfLongitudes * 6, 0, 0);
}

void SkyBox::translate(float x, float y, float z)
{
	Shape::translate(x, y, z);
}

void SkyBox::rotate(float x, float y, float z)
{
	Shape::rotate(x, y, z);
}

void SkyBox::scale(float x, float y, float z)
{
	Shape::scale(x, y, z);
}

void SkyBox::setPosition(float x, float y, float z)
{
	Shape::setPosition(x, y, z);
}

void SkyBox::setRotation(float x, float y, float z)
{
	Shape::setRotation(x, y, z);
}

void SkyBox::setScale(float x, float y, float z)
{
	Shape::setScale(x, y, z);
}
