#include "stdafx.h"
#include "GLVertexBuffer.h"

ym::GLVertexBuffer::GLVertexBuffer()
{
	YM_LOG_ERROR("Missing implementation for 'VertexBuffer' in GLVertexBuffer!");
}

ym::GLVertexBuffer::~GLVertexBuffer()
{
}

void ym::GLVertexBuffer::setData(const void* data, unsigned int size, Usage usage)
{
	YM_LOG_ERROR("Missing implementation for 'setData' in GLVertexBuffer!");
}

void ym::GLVertexBuffer::bind()
{
	YM_LOG_ERROR("Missing implementation for 'bind' in GLVertexBuffer!");
}

void* ym::GLVertexBuffer::getBuffer()
{
	YM_LOG_ERROR("Missing implementation for 'getBuffer' in GLVertexBuffer!");
	return nullptr; // Should return the id of the buffer.
}

unsigned int ym::GLVertexBuffer::getSize()
{
	YM_LOG_ERROR("Missing implementation for 'getSize' in GLVertexBuffer!");
	return 0;
}
