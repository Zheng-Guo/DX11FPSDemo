#include <d3dx11.h>
#include "Material.h"

Material::Material(ID3D11Device* d, LPCWSTR e, LPCWSTR t) :device(d),
effectFileName(e),
textureFileName(t)
{
	D3DX11CompileFromFile(effectFileName, nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &vertexShaderBuffer, nullptr, nullptr);
	D3DX11CompileFromFile(effectFileName, nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &pixelShaderBuffer, nullptr, nullptr);
	device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
	device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);

	D3DX11CreateShaderResourceViewFromFile(device, textureFileName, nullptr, nullptr, &texture, nullptr);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &textureSamplerState);
}

Material::~Material()
{
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
	if (vertexShaderBuffer) vertexShaderBuffer->Release();
	if (pixelShaderBuffer) pixelShaderBuffer->Release();
	if (texture) texture->Release();
	if (textureSamplerState) textureSamplerState->Release();
}
