#pragma once

#include "../../Engine/Core/Graphics/VertexArray.h"
#include "GLAPI.h"

namespace ym
{
	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray();
		virtual ~GLVertexArray(); 

		void addBuffer(VertexBuffer* vb, const AttributeLayout& layout) override;
		void bind() override;

	private:
		unsigned m_id;
		unsigned m_nextLocation;

		std::vector<VertexBuffer*> m_vbos;
	};
}