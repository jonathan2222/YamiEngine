#include "stdafx.h"
#include "DX11VertexArray.h"
#include "DX11API.h"

ym::DX11VertexArray::DX11VertexArray()
{
}

ym::DX11VertexArray::~DX11VertexArray()
{
	YM_PROFILER_FUNCTION();

	for (DX11VertexBuffer* vb : m_vbs)
		delete vb;
	m_vbs.clear();
	m_buffers.clear();
	m_strides.clear();
}

void ym::DX11VertexArray::addBuffer(VertexBuffer* vb, const AttributeLayout& layout)
{
	YM_PROFILER_FUNCTION();

	m_vbs.push_back(dynamic_cast<DX11VertexBuffer*>(vb));
	m_buffers.push_back((ID3D11Buffer*)vb->getBuffer());
	m_strides.push_back(layout.getStride());
	m_offsets.push_back(0);
}

void ym::DX11VertexArray::bind()
{
	YM_PROFILER_RENDERING_FUNCTION();

	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();

	context->IASetVertexBuffers(0, (UINT)m_buffers.size(), m_buffers.data(), m_strides.data(), m_offsets.data());
}
