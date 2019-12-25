#pragma once

#include "../../Engine/Core/Graphics/VertexArray.h"

namespace ym
{
	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray();
		virtual ~GLVertexArray(); 

		void addBuffer(VertexBuffer* vb, const AttributeLayout& layout) override;
		void bind() override;
	};
}