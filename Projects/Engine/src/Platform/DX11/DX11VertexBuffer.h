#pragma once

#include "../../Engine/Core/Graphics/VertexBuffer.h"
#include "DX11API.h"

namespace ym
{
	class DX11VertexBuffer : public VertexBuffer
	{
	public:
		DX11VertexBuffer();
		virtual ~DX11VertexBuffer();

		void setData(const void* data, unsigned int size, Usage usage) override;

		void bind() override;

		void* getBuffer() override;
		unsigned int getSize() override;

	private:
		unsigned int m_size;
		ID3D11Buffer* m_buffer;
	};
}