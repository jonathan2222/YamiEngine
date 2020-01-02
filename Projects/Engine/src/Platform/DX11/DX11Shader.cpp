#include "stdafx.h"
#include "DX11Shader.h"
#include <iostream>

ym::DX11Shader::DX11Shader() : m_psShader(nullptr), m_vsShader(nullptr), 
	m_layout(nullptr), m_vertexShaderBuffer(nullptr), m_psReflector(nullptr), m_vsReflector(nullptr)
{
}

ym::DX11Shader::~DX11Shader()
{
	for (ID3D11ShaderResourceView* srv : m_srvs)
		srv->Release();
	m_srvs.clear();

	for (ID3D11SamplerState* sampler : m_samplers)
		sampler->Release();
	m_samplers.clear();

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_psReflector)
	{
		m_psReflector->Release();
		m_psReflector = nullptr;
	}

	if (m_vsReflector)
	{
		m_vsReflector->Release();
		m_vsReflector = nullptr;
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

void ym::DX11Shader::load(const std::string& fileName, AttributeLayout& layout)
{
	YM_PROFILER_FUNCTION();

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
	YM_PROFILER_RENDERING_FUNCTION();

	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	context->IASetInputLayout(m_layout);
	context->VSSetShader(m_vsShader, NULL, 0);
	context->PSSetShader(m_psShader, NULL, 0);
}

void* ym::DX11Shader::getId()
{
	// The DX11 implementation does not have an id!
	return nullptr;
}

void ym::DX11Shader::setTexture(const std::string& name, Texture* texture, Sampler sampler, unsigned int unit)
{
	YM_PROFILER_RENDERING_FUNCTION();

	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();

	// Fetch the SRV.
	UniformData* uDataRTV = addUniform(name);
	if (uDataRTV)
	{
		YM_PROFILER_RENDERING_SCOPE("Bind/Create RTV");

		// Create the SRV if it was just added.
		if (uDataRTV->justAdded)
		{
			YM_LOG_WARN("Create RTV");

			uDataRTV->justAdded = false;
			HRESULT result;
			ID3D11Device* device = DX11API::get()->getDevice();

			ID3D11ShaderResourceView* srv = nullptr;
			// Create a new SRV.
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DX11API::get()->convertFormat(texture->getFormat());
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			result = device->CreateShaderResourceView((ID3D11Texture2D*)texture->getBuffer(), &srvDesc, &srv);
			YM_DX11_ASSERT_CHECK(result, "Failed to create SRV for the uniform texture {0}!", name.c_str());

			// Add SRV to the list of SRVs.
			uDataRTV->index = (unsigned int)m_srvs.size();
			m_srvs.push_back(srv);
		}

		// Bind the texture.
		ID3D11ShaderResourceView* srv = m_srvs[uDataRTV->index];
		if ((uDataRTV->shaderType & YM_SHADER_TYPE_VERTEX) != 0)
			context->VSSetShaderResources(uDataRTV->binding.BindPoint, 1, &srv);
		if ((uDataRTV->shaderType & YM_SHADER_TYPE_PIXEL) != 0)
			context->PSSetShaderResources(uDataRTV->binding.BindPoint, 1, &srv);
	}

	// Fetch the sampler.
	std::string samplerName = name + "Sampler";
	UniformData* uDataSampler = addUniform(samplerName);
	if (uDataSampler)
	{
		YM_PROFILER_RENDERING_SCOPE("Bind/Create Sampler");

		// Create the sampler if it was just added.
		if (uDataSampler->justAdded)
		{
			YM_LOG_WARN("Create Sampler");

			uDataSampler->justAdded = false;
			HRESULT result;
			ID3D11Device* device = DX11API::get()->getDevice();

			ID3D11SamplerState* sampleState = nullptr;

			// Create sampler.
			D3D11_TEXTURE_ADDRESS_MODE addressMode = DX11API::get()->convertSamplerAddressMode(sampler.addressMode);
			D3D11_SAMPLER_DESC samplerDesc;
			samplerDesc.Filter = DX11API::get()->convertFilter(sampler.filter);
			samplerDesc.AddressU = addressMode;
			samplerDesc.AddressV = addressMode;
			samplerDesc.AddressW = addressMode;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			result = device->CreateSamplerState(&samplerDesc, &sampleState);
			YM_DX11_ASSERT_CHECK(result, "Failed to create Sampler for the uniform texture {0}!", samplerName.c_str());

			// Add sampler to the list of samplers.
			uDataSampler->index = (unsigned int)m_samplers.size();
			m_samplers.push_back(sampleState);
		}

		// Bind the sampler.
		ID3D11SamplerState* sampleState = m_samplers[uDataSampler->index];
		if ((uDataSampler->shaderType & YM_SHADER_TYPE_VERTEX) != 0)
			context->VSSetSamplers(uDataSampler->binding.BindPoint, 1, &sampleState);
		if ((uDataSampler->shaderType & YM_SHADER_TYPE_PIXEL) != 0)
			context->PSSetSamplers(uDataSampler->binding.BindPoint, 1, &sampleState);
	}
}

ym::DX11Shader::UniformData* ym::DX11Shader::addUniform(const std::string& name)
{
	// Check if the uniform already exists.
	if (m_uniforms.find(name) == m_uniforms.end())
	{
		HRESULT result;
		UniformData uData;
		uData.justAdded = true;
		uData.shaderType = 0;

		// Try to fetch its binding info from the pixel shader.
		D3D11_SHADER_INPUT_BIND_DESC binding;
		result = m_psReflector->GetResourceBindingDescByName(name.c_str(), &binding);
		if (SUCCEEDED(result))
		{
			YM_LOG_WARN("GetResourceBindingDescByName on pixel shader return TRUE!");
			uData.shaderType = YM_SHADER_TYPE_PIXEL;
			uData.binding = binding;
		}

		if (uData.shaderType != 0)
		{
			// Try to fetch its binding info from the vertex shader.
			result = m_vsReflector->GetResourceBindingDescByName(name.c_str(), &binding);
			if (SUCCEEDED(result))
			{
				YM_LOG_WARN("GetResourceBindingDescByName on vertex shader return TRUE!");
				uData.shaderType = YM_SHADER_TYPE_VERTEX;
				uData.binding = binding;
			}
		}

		// Log if it did not exist and return a nullptr.
		if (uData.shaderType == 0)
		{
			YM_LOG_WARN("Can't find uniform '{0}'!", name.c_str());
			return nullptr;
		}

		// Add it to the hash map.
		m_uniforms.insert({ name, uData });
	}

	// Return its binding info.
	UniformData& uniformData = m_uniforms[name];
	return &uniformData;
}

void ym::DX11Shader::createPixelShader(const std::string& path)
{
	YM_PROFILER_FUNCTION();

	HRESULT result;
	ID3DBlob* pixelShaderBuffer = nullptr;
	ID3DBlob* errorMessageBlob = nullptr;
	std::wstring p = s2ws(path);
	LPCWSTR pathW = p.c_str();

	result = D3DCompileFromFile(pathW, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessageBlob);
	if (FAILED(result))
	{
		if (errorMessageBlob)
			compileShader(errorMessageBlob);
		else
			YM_ASSERT(false, "Failed to initialize pixel shader: Missing file: {0}", path.c_str());
	}

	ID3D11Device* device = DX11API::get()->getDevice();
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), 0, &m_psShader);
	YM_DX11_ASSERT_CHECK(result, "Failed to create pixel shader!");

	// Create reflection
	result = D3DReflect(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&m_psReflector);
	YM_DX11_ASSERT_CHECK(result, "Failed to create pixel shader reflection!");

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
}

void ym::DX11Shader::createVertexShader(const std::string& path)
{
	YM_PROFILER_FUNCTION();

	HRESULT result;
	ID3DBlob* errorMessageBlob = nullptr;
	std::wstring p = s2ws(path);
	LPCWSTR pathW = p.c_str();

	// Compile vertex shader.
	result = D3DCompileFromFile(pathW, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_vertexShaderBuffer, &errorMessageBlob);
	if (FAILED(result))
	{
		if (errorMessageBlob)
			compileShader(errorMessageBlob);
		else
			YM_ASSERT(false, "Failed to initialize vertex shader: Missing file: {0}", path.c_str());
	}

	ID3D11Device* device = DX11API::get()->getDevice();
	result = device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), 0, &m_vsShader);
	YM_DX11_ASSERT_CHECK(result, "Failed to create vertex shader!");

	// Create reflection
	result = D3DReflect(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)& m_vsReflector);
	YM_DX11_ASSERT_CHECK(result, "Failed to create vertex shader reflection!");
}

void ym::DX11Shader::compileShader(ID3DBlob* errorMessageBlob)
{
	YM_PROFILER_FUNCTION();

	// Get a pointer to the error message text buffer.
	char* compileErrors = (char*)(errorMessageBlob->GetBufferPointer());

	// Write out the error message.
	YM_LOG_ERROR(compileErrors);

	// Release the error message.
	errorMessageBlob->Release();
	errorMessageBlob = 0;

	YM_ASSERT(false, "Failed to compile shader!");
}

void ym::DX11Shader::createLayout(AttributeLayout& layout)
{
	YM_PROFILER_FUNCTION();

	// Create the vertex input layout description.
	// This setup needs to match the Vertex stucture in used in the model and in the shader.
	size_t numElements = layout.getAttributes().size();
	const std::vector<AttributeLayout::Attribute>& attributes = layout.getAttributes();
	D3D11_INPUT_ELEMENT_DESC* polygonLayout = new D3D11_INPUT_ELEMENT_DESC[numElements];
	for (unsigned int i = 0; i < numElements; i++)
	{
		const AttributeLayout::Attribute& attribute = attributes[i];
		polygonLayout[i].SemanticName = attribute.getSemanticName().c_str();
		polygonLayout[i].SemanticIndex = 0;
		polygonLayout[i].Format = DX11API::get()->convertFormat(attribute.getFormat());
		polygonLayout[i].InputSlot = 0;
		polygonLayout[i].AlignedByteOffset = (i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT);
		polygonLayout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[i].InstanceDataStepRate = 0;
	}

	// Create the vertex input layout.
	ID3D11Device* device = DX11API::get()->getDevice();
	HRESULT result = device->CreateInputLayout(polygonLayout, (UINT)numElements, m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(), &m_layout);
	YM_DX11_ASSERT_CHECK(result, "Failed to create input layout!");

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
