#pragma once

#include "../../Engine/Core/Graphics/UniformBuffer.h"
#include "GLAPI.h"

namespace ym
{
	class GLUniformBuffer : public UniformBuffer
	{
	public:
		GLUniformBuffer();
		virtual ~GLUniformBuffer();

		void setShader(Shader& shader, unsigned int index, unsigned int bindingPoint) override;
		void setData(const void* data, unsigned int size) override;
		bool updateData(const void* data, unsigned int size, unsigned int offset) override;
		void bind(unsigned int typesMask) override;

	private:
		unsigned int m_size;
	};
}