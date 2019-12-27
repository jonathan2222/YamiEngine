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

	m_vsShader = nullptr;
	m_psShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
}

void ym::DX11Renderer::init(DisplayDesc& displayDescriptor)
{
	m_psShader = 0;
	m_vsShader = 0;
	m_layout = 0;

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

	// TODO: Create camera stuff.
}

void ym::DX11Renderer::destroy()
{
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_psShader)
	{
		m_psShader->Release();
		m_psShader = 0;
	}

	if (m_vsShader)
	{
		m_vsShader->Release();
		m_vsShader = 0;
	}

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

void ym::DX11Renderer::initShader(WCHAR* vertexShader, WCHAR* pixelShader)
{
	/*
	HRESULT result;
	ID3DBlob* vertexShaderBuffer = nullptr;
	ID3DBlob* errorMessageBlob = nullptr;

	// Compile vertex shader.
	result = D3DCompileFromFile(vertexShader, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessageBlob);
	if (FAILED(result))
	{
		if (errorMessageBlob)
			compileShader(errorMessageBlob, vertexShader);
		else
			YM_ASSERT(false, "Failed to initialize vertex shader: Missing file!");
	}

	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 0, &m_vsShader);
	YM_ASSERT(FAILED(result) == false, "Failed to create vertex shader!");

	// Compile pixel shader.
	ID3DBlob* pixelShaderBuffer = nullptr;
	errorMessageBlob = nullptr;
	result = D3DCompileFromFile(pixelShader, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessageBlob);
	if (FAILED(result))
	{
		if (errorMessageBlob)
			compileShader(errorMessageBlob, pixelShader);
		else
			YM_ASSERT(false, "Failed to initialize pixel shader: Missing file!");
	}

	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), 0, &m_psShader);
	YM_ASSERT(FAILED(result) == false, "Failed to create pixel shader!");

	// Create the vertex input layout description.
	// This setup needs to match the Vertex stucture in used int the model and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	SIZE_T numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = m_device->CreateInputLayout(polygonLayout, (UINT)numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	YM_ASSERT(FAILED(result) == false, "Failed to create input layout!");

	// ------------------------------ Release shader blob code ------------------------------
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// ------------------------------ Contant buffer ------------------------------
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	YM_ASSERT(FAILED(result) == false, "Failed to create a constant buffer for the matrtices!");
	*/
}

void ym::DX11Renderer::bindShader(glm::mat4& world, glm::mat4& view, glm::mat4& proj)
{
	/*
	// Update constant buffer.
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = m_context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	YM_ASSERT(FAILED(result) == false, "Failed to map (lock) constant buffer!");

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = glm::transpose(world);
	dataPtr->view = glm::transpose(view);
	dataPtr->projection = glm::transpose(proj);

	// Unlock the constant buffer.
	m_context->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	m_context->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Bind shader.
	m_context->IASetInputLayout(m_layout);
	m_context->VSSetShader(m_vsShader, NULL, 0);
	m_context->PSSetShader(m_psShader, NULL, 0);
	*/
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

void ym::DX11Renderer::compileShader(ID3DBlob* errorMessageBlob, WCHAR* fileName)
{
	// Get a pointer to the error message text buffer.
	char* compileErrors = (char*)(errorMessageBlob->GetBufferPointer());

	// Get the length of the message.
	//unsigned long bufferSize = errorMessageBlob->GetBufferSize();

	// Write out the error message.
	YM_LOG_ERROR(compileErrors);

	// Release the error message.
	errorMessageBlob->Release();
	errorMessageBlob = 0;

	YM_ASSERT(false, "Failed to compile shader!");
}
