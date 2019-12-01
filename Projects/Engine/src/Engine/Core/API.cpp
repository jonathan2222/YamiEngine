#include "API.h"

#include "../Defines.h"

#if YM_CURRENT_API_TYPE == YM_API_GL
	#include "../../Platform/GL/GLAPI.h"
#elif YM_CURRENT_API_TYPE == YM_API_DX11
	#include "../../Platform/DX11/DX11API.h"
#endif

ym::API* ym::API::get()
{
#if YM_CURRENT_API_TYPE == YM_API_GL
	YM_LOG_INFO("Using API: OpenGL");
	return GLAPI::get();
#elif YM_CURRENT_API_TYPE == YM_API_DX11
	YM_LOG_INFO("Using API: DirectX11");
	return DX11API::get();
#endif
}

ym::API::VideoCardInfo& ym::API::getVideoCardInfo()
{
	return m_videoCardinfo;
}
