#include "stdafx.h"
#include "GLIndexBuffer.h"

ym::GLIndexBuffer::GLIndexBuffer() : m_id(0)
{
}

ym::GLIndexBuffer::~GLIndexBuffer()
{
	YM_PROFILER_FUNCTION();

	if (glIsBuffer(m_id) == GL_TRUE)
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}

void ym::GLIndexBuffer::setData(const void* data, unsigned int count, Usage usage)
{
	YM_PROFILER_FUNCTION();

	if (glIsBuffer(m_id) == GL_TRUE)
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
	m_count = count;
	
	// Create buffer.
	glGenBuffers(1, &m_id);
	
	// Bind.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

	// Set the data for the buffer.
	GLenum glUsage = GL_STATIC_DRAW;
	if(usage == Usage::STATIC) glUsage = GL_STATIC_DRAW;
	if(usage == Usage::DYNAMIC) glUsage = GL_DYNAMIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, glUsage);

	// Unbind.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ym::GLIndexBuffer::bind()
{
	YM_PROFILER_RENDERING_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}
