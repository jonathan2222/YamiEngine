#include "stdafx.h"
#include "ResourceManager.h"

#pragma warning( push )
#pragma warning( disable : 6011 )
#pragma warning( disable : 6262 )
#pragma warning( disable : 6308 )
#pragma warning( disable : 6387 )
#pragma warning( disable : 26451 )
#pragma warning( disable : 28182 )
#define STB_IMAGE_IMPLEMENTATION
#include "../../../Utils/stb_image.h"
#pragma warning( pop )

ym::ResourceManager& ym::ResourceManager::get()
{
	static ResourceManager resourceManager;
	return resourceManager;
}

ym::ResourceManager::Image* ym::ResourceManager::loadImage(const std::string& fileName, int nChannels)
{
	std::string path = std::string(YM_TEXTURE_PATH) + fileName;
	Image* img = new Image();
	int width = 0, height = 0, channelCount = 0;
	if (nChannels < 0 || nChannels > 4)
	{
		img->data = nullptr;
		YM_LOG_WARN("Unable to load texture [{0}]: Requested number of channels is not supported! Requested {1}.", path.c_str(), nChannels);
	}
	else
	{
		img->data = stbi_load(path.c_str(), &width, &height, &channelCount, nChannels);
	}

	img->width = (unsigned int)width;
	img->height = (unsigned int)height;
	img->nChannels = (unsigned int)nChannels;

	if (!img->data)
	{
		YM_LOG_WARN("Unable to load texture [{0}]: file not found!", path.c_str());
	}

	return img;
}

void ym::ResourceManager::freeImage(Image* image)
{
	if (image)
	{
		if (image->data)
			stbi_image_free(image->data);
		delete image;
	}
	else
		YM_LOG_WARN("Trying to free an image pointer which is NULL!");
}
