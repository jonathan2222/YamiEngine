#include "stdafx.h"
#include "GLUniformBuffer.h"

ym::GLUniformBuffer::GLUniformBuffer() : m_size(0), m_id(0), m_bindingPoint(0)
{

}

ym::GLUniformBuffer::~GLUniformBuffer()
{
	YM_PROFILER_FUNCTION();

	if (glIsBuffer(m_id) == GL_TRUE)
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}

void ym::GLUniformBuffer::setShader(Shader& shader, unsigned int index, unsigned int bindingPoint)
{
	YM_PROFILER_FUNCTION();

	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	m_bindingPoint = bindingPoint;
	GLuint shaderID = *(GLuint*)shader.getId();
	glUniformBlockBinding(shaderID, index, bindingPoint);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ym::GLUniformBuffer::setData(const void* data, unsigned int size)
{
	YM_PROFILER_FUNCTION();

	if (glIsBuffer(m_id) == GL_TRUE)
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
	m_size = size;

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);

	glBufferData(GL_UNIFORM_BUFFER, m_size, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_id);
}

bool ym::GLUniformBuffer::updateData(const void* data, unsigned int size, unsigned int offset)
{
	YM_PROFILER_RENDERING_FUNCTION();

	if (offset + size <= m_size) {
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, m_size, data);
		return true;
	}

	YM_LOG_WARN("Data doesn't fit in uniform buffer! Data size: {0} Free Data from offset: {1}", size, m_size - offset);
	return false;
}

void ym::GLUniformBuffer::bind(unsigned int typesMask)
{
	YM_PROFILER_RENDERING_FUNCTION();

	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
}
