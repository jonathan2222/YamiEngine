#include "stdafx.h"
#include "GLVertexArray.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

ym::GLVertexArray::GLVertexArray() : m_id(0), m_nextLocation(0)
{
	YM_PROFILER_FUNCTION();

	glGenVertexArrays(1, &m_id);
}

ym::GLVertexArray::~GLVertexArray()
{
	YM_PROFILER_FUNCTION();

	for (VertexBuffer*& vb : m_vbos)
		delete vb;
	m_vbos.clear();

	glDeleteVertexArrays(1, &m_id);
}

void ym::GLVertexArray::addBuffer(VertexBuffer* vb, const AttributeLayout& layout)
{
	YM_PROFILER_FUNCTION();

	m_vbos.push_back(vb);

	bind();
	vb->bind();
	const std::vector<AttributeLayout::Attribute>& attributes = layout.getAttributes();
	for (unsigned int i = 0; i < attributes.size(); i++) {
		const AttributeLayout::Attribute& attribute = attributes[i];
		glEnableVertexAttribArray(m_nextLocation);
		
		GLenum type = GLAPI::get()->convertType(attribute.getType());
		unsigned int offset = attribute.getOffset();
		glVertexAttribPointer(m_nextLocation, attribute.getCount(), type, GL_FALSE, layout.getStride(), BUFFER_OFFSET(offset));

		// Set instance divisor, 0 if no instance, 1 if it should change per instance, 2 if every seconds instance and so on.
		if(attribute.getInstanceDivisor() != 0)
			glVertexAttribDivisor(m_nextLocation, attribute.getInstanceDivisor());

		m_nextLocation++;
	}
}

void ym::GLVertexArray::bind()
{
	YM_PROFILER_RENDERING_FUNCTION();

	glBindVertexArray(m_id);
}
