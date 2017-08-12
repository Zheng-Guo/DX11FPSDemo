#pragma once
#include <windows.h>
#include <d3d11.h>
#include "GameObject.h"
#include "UIComponent.h"

class RenderingWindow
{
private:
	int width, height;
	float backgroundColor[4];
	HWND windowHandle;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
public:
	RenderingWindow(int w = 100, int h = 100, HWND wh = nullptr);
	~RenderingWindow();
	bool initD3D();
	void setBackgroundColor(float r, float g, float b, float a);
	bool initRendering();
	ID3D11Device* getDevice() const { return device; }
	void clear();
	void draw(Shape *s, CXMMATRIX camView, CXMMATRIX camProjection);
	void draw(GameObject *o, CXMMATRIX camView, CXMMATRIX camProjection);
	void draw(UIComponent *u, CXMMATRIX camView, CXMMATRIX camProjection);
	void display() const;
};