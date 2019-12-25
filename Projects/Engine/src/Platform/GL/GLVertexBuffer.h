#pragma once

#include "../../Engine/Core/Graphics/VertexBuffer.h"

namespace ym
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer();
		virtual ~GLVertexBuffer();

		void setData(const void* data, unsigned int size, Usage usage) override;

		void bind() override;

		void* getBuffer() override;
		unsigned int getSize() override;
	};
}