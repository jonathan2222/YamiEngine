#include "stdafx.h"
#include "AttributeLayout.h"

ym::AttributeLayout::AttributeLayout() : m_stride(0)
{
}

void ym::AttributeLayout::push(unsigned int count, Type type)
{
	Attribute attribute(count, m_stride, type);
	m_stride += attribute.getSize();
	m_attributes.push_back(attribute);
}

unsigned int ym::AttributeLayout::getStride() const
{
	return m_stride;
}

const std::vector<ym::AttributeLayout::Attribute>& ym::AttributeLayout::getAttributes() const
{
	return m_attributes;
}

ym::AttributeLayout::Attribute::Attribute(unsigned int count, unsigned int offset, Type type)
	: m_count(count), m_offset(offset), m_type(type)
{
	m_size = sizeofType(type) * count;
}

unsigned int ym::AttributeLayout::Attribute::getCount() const
{
	return m_count;
}

unsigned int ym::AttributeLayout::Attribute::getSize() const
{
	return m_size;
}

unsigned int ym::AttributeLayout::Attribute::getOffset() const
{
	return m_offset;
}

ym::Type ym::AttributeLayout::Attribute::getType() const
{
	return m_type;
}
