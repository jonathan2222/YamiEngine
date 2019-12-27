#pragma once

#include "../../Engine/Core/Graphics/UniformBuffer.h"
#include "DX11API.h"

namespace ym
{
	class DX11UniformBuffer : public UniformBuffer
	{
	public:
		DX11UniformBuffer();
		virtual ~DX11UniformBuffer();

		void setShader(Shader& shader, unsigned int index, unsigned int bindingPoint) override;
		void setData(const void* data, unsigned int size) override;
		bool updateData(const void* data, unsigned int size, unsigned int offset) override;
		void bind(unsigned int typesMask) override;

	private:
		ID3D11Buffer* m_buffer;
		unsigned int m_bufferSlot;
		unsigned int m_size;
	};
}