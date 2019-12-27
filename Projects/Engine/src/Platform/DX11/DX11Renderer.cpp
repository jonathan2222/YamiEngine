#include "stdafx.h"
#include "DX11Renderer.h"

#include "../../Engine/Core/Display.h"
#include <glm/gtc/type_ptr.hpp>

ym::DX11Renderer* ym::DX11Renderer::get()
{
	static DX11Renderer renderer;
	return &renderer;
}

ym::DX11Renderer::DX11Renderer()
{
	m_device = nullptr;
	m_context = nullptr;
	m_swapChain = nullptr;

	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterizerState = nullptr;
}

void ym::DX11Renderer::init(DisplayDesc& displayDescriptor)
{

	// Fetch the device, device context and the swap chain from the DirectX api.
	m_device = DX11API::get()->getDevice();
	m_context = DX11API::get()->getDeviceContext();
	m_swapChain = DX11API::get()->getSwapChain();

	createRTV();
	createDepthBuffer(displayDescriptor);

	createDepthStencilState();
	// Set the depth stencil state.
	m_context->OMSetDepthStencilState(m_depthStencilState, 1);

	createDepthStencilView();
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	createRasterizer();
	// Set the rasterizer state.
	m_context->RSSetState(m_rasterizerState);

	createAndSetViewport(displayDescriptor);
}

void ym::DX11Renderer::destroy()
{
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
}

void ym::DX11Renderer::beginScene(float r, float g, float b, float a)
{
	float color[4] = {r, g, b, a};
	// Clear back buffer.
	m_context->ClearRenderTargetView(m_renderTargetView, color);
	// Clear depth buffer.
	m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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

void ym::DX11Renderer::draw(VertexArray* va, IndexBuffer* ib, Topology topology, Shader* shader)
{
	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	va->bind();
	ib->bind();
	shader->bind();
	context->IASetPrimitiveTopology(getD3D11Topology(topology));
	context->DrawIndexed(ib->getCount(), 0, 0);
}

void ym::DX11Renderer::draw(Model* model, Shader* shader)
{
	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	model->bind();
	shader->bind();
	context->IASetPrimitiveTopology(getD3D11Topology(model->getInfo().topology));
	context->DrawIndexed(model->getIndexBuffer()->getCount(), 0, 0);
}

D3D11_PRIMITIVE_TOPOLOGY ym::DX11Renderer::getD3D11Topology(Topology topology)
{
	switch (topology)
	{
	case Topology::POINT_LIST: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST; break;
	case Topology::LINE_LIST: return D3D_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case Topology::LINE_STRIP: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
	case Topology::TRIANGLE_STRIP: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
	case Topology::TRIANGLE_LIST:
	default:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}
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
	m_context->RSSetViewports(1, &viewport);
}
