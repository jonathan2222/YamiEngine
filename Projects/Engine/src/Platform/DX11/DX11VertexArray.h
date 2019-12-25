#pragma once

#include "../../Engine/Core/Graphics/VertexArray.h"
#include "DX11API.h"
#include "DX11VertexBuffer.h"

namespace ym
{
	class DX11VertexArray : public VertexArray
	{
	public:
		DX11VertexArray();
		virtual ~DX11VertexArray();

		void addBuffer(VertexBuffer* vb, const AttributeLayout& layout) override;
		void bind() override;

	private:
		std::vector<DX11VertexBuffer*> m_vbs;
		std::vector<ID3D11Buffer*> m_buffers;
		std::vector<unsigned int> m_strides;
		std::vector<unsigned int> m_offsets;
	};
}