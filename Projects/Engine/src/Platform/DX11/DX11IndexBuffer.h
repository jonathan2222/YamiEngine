#pragma once

#include "../../Engine/Core/Graphics/IndexBuffer.h"
#include "DX11API.h"

namespace ym
{
	class DX11IndexBuffer : public IndexBuffer
	{
	public:
		DX11IndexBuffer();
		virtual ~DX11IndexBuffer();

		void setData(const void* data, unsigned int count, Usage usage) override;
		void bind() override;
	
	private:
		ID3D11Buffer* m_buffer;
	};
}