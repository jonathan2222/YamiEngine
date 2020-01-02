#include "stdafx.h"
#include "DX11Texture.h"

#include "../../Engine/Core/Graphics/Shader.h"

ym::DX11Texture::DX11Texture() : m_resourceSlot(0), m_samplerSlot(0), m_texture(nullptr)
{
	m_image = ResourceManager::Image();
	m_format = Format::UINT_32_RGBA;
	m_sampler = Sampler();
}

ym::DX11Texture::~DX11Texture()
{
	YM_PROFILER_FUNCTION();

	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

void ym::DX11Texture::setData(ResourceManager::Image* img, Format format, Sampler sampler, Usage usage)
{
	YM_PROFILER_FUNCTION();

	// Copy information data.
	m_image = *img;
	m_image.data = nullptr;
	m_format = format;
	m_sampler = sampler;

	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	HRESULT result;
	ID3D11Device* device = DX11API::get()->getDevice();

	const Type type = typeOfFormat(format);
	const unsigned int formatCount = countOfFormat(format);
	const unsigned int bytesPerPixel = sizeOfType(type) * formatCount;

	// Create the Texture2D object.
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = img->width;
	texDesc.Height = img->height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = (usage == Usage::STATIC ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DEFAULT);
	texDesc.Format = DX11API::get()->convertFormat(format);
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA texInitData;
	ZeroMemory(&texInitData, sizeof(texInitData));
	texInitData.pSysMem = (void*)img->data;
	texInitData.SysMemPitch = img->width * bytesPerPixel;
	texInitData.SysMemSlicePitch = img->width * img->height * bytesPerPixel; // Not needed since this is a 2d texture

	// Create the object.
	result = device->CreateTexture2D(&texDesc, &texInitData, &m_texture);
	YM_DX11_ASSERT_CHECK(result, "Failed to create a Texture2D!");
}

void* ym::DX11Texture::getBuffer()
{
	return (void*)m_texture;
}

ym::ResourceManager::Image ym::DX11Texture::getImageInfo()
{
	return m_image;
}

ym::Format ym::DX11Texture::getFormat()
{
	return m_format;
}

ym::Sampler ym::DX11Texture::getSampler()
{
	return m_sampler;
}
