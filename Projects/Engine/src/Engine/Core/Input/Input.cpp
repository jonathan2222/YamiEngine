#include "Input.h"

#include "../../Defines.h"
#include "Config.h"

#include "../../../Platform/GL/GLInput.h"
#ifdef YAMI_PLATFORM_WINDOWS
	#include "../../../Platform/DX11/DX11Input.h"
#endif

ym::Input* ym::Input::m_self = nullptr;

ym::Input* ym::Input::get()
{
	return m_self;
}

ym::Input* ym::Input::create()
{
	if (m_self != nullptr) delete m_self;

	// Convert keys to match the API.
	KeyConverter::init();

	static std::string type = Config::get()->fetch<std::string>("API/type");
	if (type == YM_API_GL) return m_self = new GLInput();
	else if (type == YM_API_DX11) return m_self = new DX11Input();

	return m_self;
}