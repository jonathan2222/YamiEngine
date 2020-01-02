#include "stdafx.h"
#include "DX11UniformBuffer.h"

ym::DX11UniformBuffer::DX11UniformBuffer() : m_bufferSlot(0), m_buffer(nullptr), m_size(0)
{
}

ym::DX11UniformBuffer::~DX11UniformBuffer()
{
	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}
}

void ym::DX11UniformBuffer::setShader(Shader& shader, unsigned int index, unsigned int bindingPoint)
{
	m_bufferSlot = index;
}

void ym::DX11UniformBuffer::setData(const void* data, unsigned int size)
{
	YM_PROFILER_FUNCTION();

	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}
	m_size = size;

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = size;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	ID3D11Device* device = DX11API::get()->getDevice();
	HRESULT result = device->CreateBuffer(&matrixBufferDesc, &initData, &m_buffer);
	YM_DX11_ASSERT_CHECK(result, "Failed to create a constant buffer!");
}

bool ym::DX11UniformBuffer::updateData(const void* data, unsigned int size, unsigned int offset)
{
	YM_PROFILER_RENDERING_FUNCTION();

	if (offset + size <= m_size)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();

		HRESULT result = context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		YM_DX11_ASSERT_CHECK(result, "Failed to map (lock) constant buffer!");

		// Get a pointer to the data in the constant buffer.
		unsigned char* dataPtr = (unsigned char*)mappedResource.pData;
		memcpy((void*)(dataPtr+offset), data, size);

		// Unlock the constant buffer.
		context->Unmap(m_buffer, 0);

		return true;
	}
	YM_LOG_WARN("Data doesn't fit in uniform buffer! Given data size: {0}, Free data from offset: {1}", size, m_size - offset);
	return false;
}

void ym::DX11UniformBuffer::bind(unsigned int typesMask)
{
	YM_PROFILER_RENDERING_FUNCTION();

	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	if ((typesMask & YM_SHADER_TYPE_VERTEX) != 0)
		context->VSSetConstantBuffers(m_bufferSlot, 1, &m_buffer);
	if ((typesMask & YM_SHADER_TYPE_PIXEL) != 0)
		context->PSSetConstantBuffers(m_bufferSlot, 1, &m_buffer);
}
