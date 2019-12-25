#pragma once

#include "../../Engine/Core/Graphics/IndexBuffer.h"

namespace ym
{
	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer();
		virtual ~GLIndexBuffer();

		void setData(const void* data, unsigned int count, Usage usage) override;
		void bind() override;
	};
}