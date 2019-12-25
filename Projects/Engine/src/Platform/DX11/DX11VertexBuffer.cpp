#include "stdafx.h"
#include "DX11VertexBuffer.h"
#include "DX11API.h"

ym::DX11VertexBuffer::DX11VertexBuffer() : m_buffer(nullptr)
{
}

ym::DX11VertexBuffer::~DX11VertexBuffer()
{
	if (m_buffer)
		m_buffer->Release();
}

void ym::DX11VertexBuffer::setData(const void* data, unsigned int size, Usage usage)
{
	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}

	ID3D11Device* device = DX11API::get()->getDevice();

	HRESULT result;

	m_size = size;

	// Create a description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.Usage = (usage == Usage::STATIC ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// Set the data.
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Create it.
	result = device->CreateBuffer(&bufferDesc, &initData, &m_buffer);
	YM_ASSERT(FAILED(result) == false, "Failed to create vertex buffer!");
}

void ym::DX11VertexBuffer::bind()
{
}

void* ym::DX11VertexBuffer::getBuffer()
{
	return (void*)m_buffer;
}

unsigned int ym::DX11VertexBuffer::getSize()
{
	return m_size;
}
