#include "stdafx.h"
#include "DX11Model.h"

#include <glm/gtc/type_ptr.hpp>

ym::DX11Model::DX11Model() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_vertexCount(0), m_indexCount(0)
{
}

void ym::DX11Model::setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	ID3D11Device* device = DX11API::get()->getDevice();
	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();

	HRESULT result;

	// Create the vertex buffer.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = glm::value_ptr(vertices[0].pos);
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&bufferDesc, &initData, &m_vertexBuffer);
	YM_ASSERT(FAILED(result) == false, "Failed to create vertex buffer!");
	m_vertexCount = (unsigned int)vertices.size();

	// Create the index bufffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initDataIndex;
	initDataIndex.pSysMem = indices.data();
	initDataIndex.SysMemPitch = 0;
	initDataIndex.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &initDataIndex, &m_indexBuffer);
	YM_ASSERT(FAILED(result) == false, "Failed to create index buffer!");
	m_indexCount = (unsigned int)indices.size();
}

void ym::DX11Model::bind()
{
	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ym::DX11Model::draw()
{
	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	context->DrawIndexed(m_indexCount, 0, 0);
}

void ym::DX11Model::destroy()
{
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
}

unsigned int ym::DX11Model::getVertexCount()
{
	return m_vertexCount;
}

unsigned int ym::DX11Model::getIndexCount()
{
	return m_indexCount;
}
