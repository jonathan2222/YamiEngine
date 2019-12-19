#include "Renderer.h"

#include "../Input/Config.h"
#include "../../../Platform/GL/GLRenderer.h"
#ifdef YAMI_PLATFORM_WINDOWS
	#include "../../../Platform/DX11/DX11Renderer.h"
#endif

ym::Renderer* ym::Renderer::get()
{
	static std::string type = Config::get()->fetch<std::string>("API/type");
	if (type == YM_API_GL) return GLRenderer::get();
	else if (type == YM_API_DX11) return DX11Renderer::get();
	YM_ASSERT(false, "Could not fetch Renderer: API not supported!");
	return nullptr;
}
