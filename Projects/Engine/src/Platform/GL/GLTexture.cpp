#include "stdafx.h"
#include "GLTexture.h"

ym::GLTexture::GLTexture() : m_id(0)
{
	m_image = ResourceManager::Image();
	m_format = Format::UINT_32_RGBA;
	m_sampler = Sampler();
}

ym::GLTexture::~GLTexture()
{
	YM_PROFILER_FUNCTION();

	if (m_id != 0)
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
}

void ym::GLTexture::setData(ResourceManager::Image* img, Format format, Sampler sampler, Usage usage)
{
	YM_PROFILER_FUNCTION();

	// Copy information data.
	m_image = *img;
	m_image.data = nullptr;
	m_format = format;
	m_sampler = sampler;

	if (m_id != 0)
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}

	// Create texture.
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// Set sampler.
	setFilter(sampler.filter);
	setAddressMode(sampler.addressMode);

	// Set data.
	//GLint glInternalFormat = GLAPI::get()->convertInternalFormat(format);
	GLenum glFormat = GLAPI::get()->convertFormat(format);
	GLenum glType = GLAPI::get()->convertType(typeOfFormat(format));
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, (GLsizei)img->width, (GLsizei)img->height, 0, glFormat, glType, (void*)img->data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void* ym::GLTexture::getBuffer()
{
	return &m_id;
}

ym::ResourceManager::Image ym::GLTexture::getImageInfo()
{
	return m_image;
}

ym::Format ym::GLTexture::getFormat()
{
	return m_format;
}

ym::Sampler ym::GLTexture::getSampler()
{
	return m_sampler;
}

void ym::GLTexture::setFilter(Sampler::Filter filter)
{
	switch(filter)
	{
	case Sampler::Filter::MIN_NEAREST_MAG_NEAREST_MIP_NEAREST:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		break;
	case Sampler::Filter::MIN_LINEAR_MAG_NEAREST_MIP_NEAREST:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		break;
	case Sampler::Filter::MIN_NEAREST_MAG_LINEAR_MIP_NEAREST:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
	case Sampler::Filter::MIN_LINEAR_MAG_LINEAR_MIP_NEAREST:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
	case Sampler::Filter::MIN_NEAREST_MAG_NEAREST_MIP_LINEAR:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		break;
	case Sampler::Filter::MIN_LINEAR_MAG_NEAREST_MIP_LINEAR:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		break;
	case Sampler::Filter::MIN_NEAREST_MAG_LINEAR_MIP_LINEAR:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
	case Sampler::Filter::MIN_LINEAR_MAG_LINEAR_MIP_LINEAR:
	default:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
	}
}

void ym::GLTexture::setAddressMode(Sampler::AddressMode addressMode)
{
	switch (addressMode)
	{
	case Sampler::AddressMode::CLAMP_TO_EDGE:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		break;
	case Sampler::AddressMode::MIRRORED_REPEAT:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		}
		break;
	case Sampler::AddressMode::REPEAT:
	default:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		break;
	}
}
