#include "stdafx.h"
#include "GLModel.h"

ym::GLModel::GLModel() : m_vertexCount(0), m_indexCount(0)
{
}

void ym::GLModel::setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
}

void ym::GLModel::bind()
{
}

void ym::GLModel::draw()
{
}

void ym::GLModel::destroy()
{
}

unsigned int ym::GLModel::getVertexCount()
{
	return m_vertexCount;
}

unsigned int ym::GLModel::getIndexCount()
{
	return m_indexCount;
}
