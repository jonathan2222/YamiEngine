#pragma once

#include "../../Engine/Core/Graphics/Texture.h"
#include "GLAPI.h"

namespace ym
{
	class GLTexture : public Texture
	{
	public:
		GLTexture();
		virtual ~GLTexture();

		void setData(ResourceManager::Image* data, Format format, Sampler sampler, Usage usage) override;
		void* getBuffer() override;

		ResourceManager::Image getImageInfo() override;
		Format getFormat() override;
		Sampler getSampler() override;

	private:
		void setFilter(Sampler::Filter filter);
		void setAddressMode(Sampler::AddressMode addressMode);

		GLuint m_id;

		ResourceManager::Image m_image;
		Format m_format;
		Sampler m_sampler;
	};
}
