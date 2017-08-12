#pragma once
#include <d3d11.h>

class Material
{
	ID3D11Device* device;
	ID3D11Buffer* constantBufferPerObject;
	ID3D11Buffer* constantBufferPerFrame;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D11ShaderResourceView* texture;
	ID3D11SamplerState* textureSamplerState;
	LPCWSTR effectFileName;
	LPCWSTR textureFileName;
public:
	Material(ID3D11Device* d, LPCWSTR e = L"DefaultEffectFile.fx", LPCWSTR t = L"DefaultTextureFile.jpg");
	~Material();
	ID3D11VertexShader* getVertexShader() const { return vertexShader; }
	ID3D11PixelShader* getPixelShader() const { return pixelShader; }
	ID3D10Blob* getVertexShaderBuffer() const { return vertexShaderBuffer; }
	ID3D10Blob* getPixelShaderBuffer() const { return pixelShaderBuffer; }
	ID3D11ShaderResourceView* getTexture() const { return texture; }
	ID3D11SamplerState* getTextureSampleState() const { return textureSamplerState; }
};