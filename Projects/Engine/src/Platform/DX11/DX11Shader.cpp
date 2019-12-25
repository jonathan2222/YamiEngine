#include "stdafx.h"
#include "DX11Shader.h"

ym::DX11Shader::DX11Shader()
{
	YM_LOG_ERROR("Missing implementation of 'Shader' in DX11Shader!");
}

ym::DX11Shader::~DX11Shader()
{
}

void ym::DX11Shader::load(const std::string& fileName, ShaderLayout layout)
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
	*/
}

void ym::DX11Shader::bind()
{
	YM_LOG_ERROR("Missing implementation of 'bind' in DX11Shader!");
}

void ym::DX11Shader::compileShader(ID3DBlob* errorMessageBlob, WCHAR* fileName)
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
