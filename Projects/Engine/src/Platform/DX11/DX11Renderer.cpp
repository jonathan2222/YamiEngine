#include "DX11Renderer.h"

#include "../../Engine/Core/Logger.h"
#include "../../Engine/Core/Input/Config.h"
#include "../../Engine/Defines.h"

#include "../../Engine/Core/Display.h"

ym::DX11Renderer* ym::DX11Renderer::get()
{
	static DX11Renderer renderer;
	return &renderer;
}

void ym::DX11Renderer::init(DisplayDesc& displayDescriptor)
{
	createSwapChainDeviceAndContext(displayDescriptor);
	createRTV();
	createDepthBuffer(displayDescriptor);

	createDepthStencilState();
	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	createDepthStencilView();
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	createRasterizer();
	// Set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterizerState);

	createAndSetViewport(displayDescriptor);

	// TODO: Create camera stuff.
}

void ym::DX11Renderer::destroy()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, NULL);

	if (m_rasterizerState)
	{
		m_rasterizerState->Release();
		m_rasterizerState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}

void ym::DX11Renderer::beginScene(float r, float g, float b, float a)
{
	float color[4] = {r, g, b, a};
	// Clear back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	// Clear depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ym::DX11Renderer::endScene()
{
	bool vsync = Config::get()->fetch<bool>("Display/vsync");
	if (vsync)
	{
		// Lock to monitor refresh rate
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible (Swap buffers)
		m_swapChain->Present(0, 0);
	}
}

void ym::DX11Renderer::createSwapChainDeviceAndContext(DisplayDesc& displayDescriptor)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Single back buffer.
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = displayDescriptor.width;
	swapChainDesc.BufferDesc.Height = displayDescriptor.height;
	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (displayDescriptor.vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = displayDescriptor.refreshRateNumerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = displayDescriptor.refreshRateDenominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = (HWND)Display::get()->getNativeDisplay();
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (displayDescriptor.fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;
	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the swap chain, device and device context!");
}

void ym::DX11Renderer::createRTV()
{
	HRESULT result;
	ID3D11Texture2D* backBufferPtr = 0;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& backBufferPtr);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to fetch the back buffer pointer!");

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the RTV!");

	backBufferPtr->Release();
	backBufferPtr = 0;
}

void ym::DX11Renderer::createDepthBuffer(DisplayDesc& displayDescriptor)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = displayDescriptor.width;
	depthBufferDesc.Height = displayDescriptor.height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	HRESULT result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the depth buffer texture!");
}

void ym::DX11Renderer::createDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	HRESULT result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the depth stencil state!");
}

void ym::DX11Renderer::createDepthStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	HRESULT result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the depth stencil view!");
}

void ym::DX11Renderer::createRasterizer()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	HRESULT result = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the rasterizer state!");
}

void ym::DX11Renderer::createAndSetViewport(DisplayDesc& displayDescriptor)
{
	D3D11_VIEWPORT viewport;
	// Setup the viewport for rendering.
	viewport.Width = (float)displayDescriptor.width;
	viewport.Height = (float)displayDescriptor.height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);
}
