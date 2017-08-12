#include <cassert>
#include "RenderingWindow.h"

RenderingWindow::RenderingWindow(int w, int h, HWND wh) :width(w),
height(h),
windowHandle(wh)
{
	backgroundColor[0] = backgroundColor[1] = backgroundColor[2] = backgroundColor[3] = 1.0f;
}

RenderingWindow::~RenderingWindow()
{
	device->Release();
	deviceContext->Release();
	swapChain->Release();
	renderTargetView->Release();
	depthStencilView->Release();
}

bool RenderingWindow::initD3D()
{
	UINT createDeviceFlags = 0;

#if defined(DEBUG)||defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&deviceContext
	);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"D3D11CreateDevice FAILED", nullptr, 0);
		return false;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(nullptr, L"Direct3D Feature Level 11 unsupported.", nullptr, 0);
		return false;
	}

	UINT m4xMsaaQuality;
	device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = windowHandle;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = nullptr;
	device->QueryInterface(__uuidof(IDXGIDevice), ((void**)&dxgiDevice));
	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), ((void**)&dxgiAdapter));
	IDXGIFactory* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), ((void**)&dxgiFactory));
	if (FAILED(dxgiFactory->CreateSwapChain(device, &sd, &swapChain)))
	{
		MessageBox(nullptr, L"CreateSwapChain FAILED", nullptr, 0);
		return false;
	}
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	ID3D11Texture2D* mDepthStencilBuffer;
	device->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer);
	device->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &depthStencilView);

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &vp);

	return true;
}

void RenderingWindow::setBackgroundColor(float r, float g, float b, float a)
{
	backgroundColor[0] = r;
	backgroundColor[1] = g;
	backgroundColor[2] = b;
	backgroundColor[3] = a;
}

bool RenderingWindow::initRendering()
{
	return true;
}

void RenderingWindow::clear()
{
	deviceContext->ClearRenderTargetView(renderTargetView, backgroundColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderingWindow::draw(Shape *s, CXMMATRIX camView, CXMMATRIX camProjection)
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	s->draw(deviceContext, camView, camProjection);
}

void RenderingWindow::draw(GameObject * o, CXMMATRIX camView, CXMMATRIX camProjection)
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	o->draw(deviceContext, camView, camProjection);
}

void RenderingWindow::draw(UIComponent * u, CXMMATRIX camView, CXMMATRIX camProjection)
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	u->draw(deviceContext, camView, camProjection);
}

void RenderingWindow::display() const
{
	swapChain->Present(0, 0);
}
