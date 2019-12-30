#include "stdafx.h"
#include "GLVertexBuffer.h"

ym::GLVertexBuffer::GLVertexBuffer() : m_id(0), m_size(0)
{
}

ym::GLVertexBuffer::~GLVertexBuffer()
{
	YM_PROFILER_FUNCTION();

	if (glIsBuffer(m_id) == GL_TRUE)
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}

void ym::GLVertexBuffer::setData(const void* data, unsigned int size, Usage usage)
{
	YM_PROFILER_FUNCTION();

	if (glIsBuffer(m_id) == GL_TRUE)
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
	m_size = size;

	// Create buffer.
	glGenBuffers(1, &m_id);

	// Bind.
	glBindBuffer(GL_ARRAY_BUFFER, m_id);

	// Set the data for the buffer.
	GLenum glUsage = GL_STATIC_DRAW;
	if (usage == Usage::STATIC) glUsage = GL_STATIC_DRAW;
	if (usage == Usage::DYNAMIC) glUsage = GL_DYNAMIC_DRAW;
	glBufferData(GL_ARRAY_BUFFER, m_size, data, glUsage);

	// Unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ym::GLVertexBuffer::bind()
{
	YM_PROFILER_RENDERING_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void* ym::GLVertexBuffer::getBuffer()
{
	return &m_id;
}

unsigned int ym::GLVertexBuffer::getSize()
{
	return m_size;
}
