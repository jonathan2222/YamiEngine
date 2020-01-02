#pragma once

#include "../../Engine/Core/Graphics/Texture.h"
#include "DX11API.h"

namespace ym
{
	class DX11Texture : public Texture
	{
	public:
		DX11Texture();
		virtual ~DX11Texture();

		void setData(ResourceManager::Image* data, Format format, Sampler sampler, Usage usage) override;
		void* getBuffer() override;

		ResourceManager::Image getImageInfo() override;
		Format getFormat() override;
		Sampler getSampler() override;

	private:
		ResourceManager::Image m_image;
		Format m_format;
		Sampler m_sampler;

		ID3D11Texture2D* m_texture;
		unsigned int m_resourceSlot;
		unsigned int m_samplerSlot;
	};
}