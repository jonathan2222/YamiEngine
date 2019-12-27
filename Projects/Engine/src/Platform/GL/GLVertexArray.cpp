#include "stdafx.h"
#include "GLVertexArray.h"

ym::GLVertexArray::GLVertexArray() : m_id(0), m_nextLocation(0)
{
	glGenVertexArrays(1, &m_id);
}

ym::GLVertexArray::~GLVertexArray()
{
	for (VertexBuffer*& vb : m_vbos)
		delete vb;
	m_vbos.clear();

	glDeleteVertexArrays(1, &m_id);
}

void ym::GLVertexArray::addBuffer(VertexBuffer* vb, const AttributeLayout& layout)
{
	m_vbos.push_back(vb);

	bind();
	vb->bind();
	const std::vector<AttributeLayout::Attribute>& attributes = layout.getAttributes();
	for (unsigned int i = 0; i < attributes.size(); i++) {
		const AttributeLayout::Attribute& attribute = attributes[i];
		glEnableVertexAttribArray(m_nextLocation);
		
		GLenum type = GLAPI::get()->convertType(attribute.getType());
		unsigned int offset = attribute.getOffset();
		glVertexAttribPointer(m_nextLocation, attribute.getSize(), type, GL_FALSE, layout.getStride(), (void*)(&offset));
		m_nextLocation++;
	}
}

void ym::GLVertexArray::bind()
{
	glBindVertexArray(m_id);
}
