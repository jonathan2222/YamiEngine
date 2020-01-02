#include "stdafx.h"
#include "DX11IndexBuffer.h"

ym::DX11IndexBuffer::DX11IndexBuffer() : m_buffer(nullptr)
{
}

ym::DX11IndexBuffer::~DX11IndexBuffer()
{
	YM_PROFILER_FUNCTION();

	if (m_buffer)
		m_buffer->Release();
}

void ym::DX11IndexBuffer::setData(const void* data, unsigned int count, Usage usage)
{
	YM_PROFILER_FUNCTION();

	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}

	m_count = count;

	ID3D11Device* device = DX11API::get()->getDevice();
	HRESULT result;

	// Set description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * count;
	bufferDesc.Usage = (usage == Usage::STATIC ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// Set data.
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Create buffer.
	result = device->CreateBuffer(&bufferDesc, &initData, &m_buffer);
	YM_DX11_ASSERT_CHECK(result, "Failed to create index buffer!");
}

void ym::DX11IndexBuffer::bind()
{
	YM_PROFILER_RENDERING_FUNCTION();

	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	context->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
}
