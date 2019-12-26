#include "stdafx.h"
#include "DX11Shader.h"

ym::DX11Shader::DX11Shader() : m_psShader(nullptr), m_vsShader(nullptr), m_layout(nullptr), m_vertexShaderBuffer(nullptr)
{
	YM_LOG_ERROR("Missing implementation of 'Shader' in DX11Shader!");
}

ym::DX11Shader::~DX11Shader()
{
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
}

void ym::DX11Shader::load(const std::string& fileName, ShaderLayout layout)
{
	// Fetch the right shader.
	std::string finalPath = std::string(YM_SHADER_PATH) + std::string("DX11/") + fileName;
	std::string vertexShaderPath = finalPath + ".vs";
	std::string pixelShaderPath = finalPath + ".ps";
	
	createVertexShader(vertexShaderPath);
	createPixelShader(pixelShaderPath);
	createLayout(layout);
	m_vertexShaderBuffer->Release();
	m_vertexShaderBuffer = nullptr;
}

void ym::DX11Shader::bind()
{
	YM_LOG_ERROR("Missing implementation of 'bind' in DX11Shader!");
}

void ym::DX11Shader::createPixelShader(const std::string& path)
{
	HRESULT result;
	ID3DBlob* pixelShaderBuffer = nullptr;
	ID3DBlob* errorMessageBlob = nullptr;
	LPCWSTR pathW = s2ws(path).c_str();
	result = D3DCompileFromFile(pathW, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessageBlob);
	if (FAILED(result))
	{
		if (errorMessageBlob)
			compileShader(errorMessageBlob);
		else
			YM_ASSERT(false, "Failed to initialize pixel shader: Missing file!");
	}

	ID3D11Device* device = DX11API::get()->getDevice();
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), 0, &m_psShader);
	YM_ASSERT(FAILED(result) == false, "Failed to create pixel shader!");

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
}

void ym::DX11Shader::createVertexShader(const std::string& path)
{
	HRESULT result;
	ID3DBlob* errorMessageBlob = nullptr;
	LPCWSTR pathW = s2ws(path).c_str();

	// Compile vertex shader.
	result = D3DCompileFromFile(pathW, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_vertexShaderBuffer, &errorMessageBlob);
	if (FAILED(result))
	{
		if (errorMessageBlob)
			compileShader(errorMessageBlob);
		else
			YM_ASSERT(false, "Failed to initialize vertex shader: Missing file!");
	}

	ID3D11Device* device = DX11API::get()->getDevice();
	result = device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), 0, &m_vsShader);
	YM_ASSERT(FAILED(result) == false, "Failed to create vertex shader!");
}

void ym::DX11Shader::compileShader(ID3DBlob* errorMessageBlob)
{
	// Get a pointer to the error message text buffer.
	char* compileErrors = (char*)(errorMessageBlob->GetBufferPointer());

	// Write out the error message.
	YM_LOG_ERROR(compileErrors);

	// Release the error message.
	errorMessageBlob->Release();
	errorMessageBlob = 0;

	YM_ASSERT(false, "Failed to compile shader!");
}

void ym::DX11Shader::createLayout(ShaderLayout& layout)
{
	// Create the vertex input layout description.
	// This setup needs to match the Vertex stucture in used in the model and in the shader.
	D3D11_INPUT_ELEMENT_DESC* polygonLayout = new D3D11_INPUT_ELEMENT_DESC[layout.elements.size()];
	for (unsigned int i = 0; i < layout.elements.size(); i++)
	{
		ShaderLayout::Element& e = layout.elements[i];
		polygonLayout[0].SemanticName = e.semanticName.c_str();
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DX11API::get()->convertFormat(e.format);
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = (i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT);
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;
	}

	// Get a count of the elements in the layout.
	SIZE_T numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	ID3D11Device* device = DX11API::get()->getDevice();
	HRESULT result = device->CreateInputLayout(polygonLayout, (UINT)numElements, m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(), &m_layout);
	YM_ASSERT(FAILED(result) == false, "Failed to create input layout!");

	delete[] polygonLayout;
}

std::wstring ym::DX11Shader::s2ws(const std::string& s) const
{
	// s2ws code from: https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
