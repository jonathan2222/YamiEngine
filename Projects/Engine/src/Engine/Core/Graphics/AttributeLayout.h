#pragma once

#include <vector>
#include "Types.h"

namespace ym
{
	class AttributeLayout
	{
	public:
		class Attribute
		{
		public:
			Attribute(unsigned int count, unsigned int offset, Type type);

			unsigned int getCount() const;
			unsigned int getSize() const;
			unsigned int getOffset() const;
			Type getType() const;

		private:
			unsigned int m_offset = 0;
			unsigned int m_count = 0;
			unsigned int m_size;
			Type m_type;
		};

		AttributeLayout();
		void push(unsigned int count, Type type);

		unsigned int getStride() const;
		const std::vector<Attribute>& getAttributes() const;

	private:
		unsigned int m_stride;
		std::vector<Attribute> m_attributes;
	};
}
