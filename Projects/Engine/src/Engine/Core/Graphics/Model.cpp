#include "stdafx.h"
#include "Model.h"

#include "../../../Platform/GL/GLModel.h"
#ifdef YAMI_PLATFORM_WINDOWS
#include "../../../Platform/DX11/DX11Model.h"
#endif

ym::Model* ym::Model::create()
{
	static std::string type = Config::get()->fetch<std::string>("API/type");
	if (type == YM_API_GL) return new GLModel();
	else if (type == YM_API_DX11) return new DX11Model();
	YM_ASSERT(false, "Could not create Model: API not supported!");
	return nullptr;
}
