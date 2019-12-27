#include "stdafx.h"
#include "GLUniformBuffer.h"

ym::GLUniformBuffer::GLUniformBuffer() : m_size(0)
{
	YM_LOG_ERROR("Missing implementation for 'UniformBuffer' in GLUniformBuffer!");
}

ym::GLUniformBuffer::~GLUniformBuffer()
{
}

void ym::GLUniformBuffer::setShader(Shader& shader, unsigned int index, unsigned int bindingPoint)
{
	YM_LOG_ERROR("Missing implementation for 'setShader' in GLUniformBuffer!");
}

void ym::GLUniformBuffer::setData(const void* data, unsigned int size)
{
	YM_LOG_ERROR("Missing implementation for 'setData' in GLUniformBuffer!");
}

bool ym::GLUniformBuffer::updateData(const void* data, unsigned int size, unsigned int offset)
{
	YM_LOG_ERROR("Missing implementation for 'updateData' in GLUniformBuffer!");
	return false;
}

void ym::GLUniformBuffer::bind(unsigned int typesMask)
{
	YM_LOG_ERROR("Missing implementation for 'bind' in GLUniformBuffer!");
}
