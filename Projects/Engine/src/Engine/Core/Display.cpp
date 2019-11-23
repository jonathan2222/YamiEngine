#include "Display.h"

#include "Input/Config.h"
#if YM_CURRENT_API_TYPE == YM_API_GL
	#include "../../Platform/GL/GLDisplay.h"
#elif YM_CURRENT_API_TYPE == YM_API_DX11
	#include "../../Platform/DX11/DX11Display.h"
#endif

ym::DisplayDesc::DisplayDesc() : width(0), height(0), title(""), fullscreen(false)
{
	this->width = Config::get()->fetch<int>("Display/defaultWidth");
	this->height = Config::get()->fetch<int>("Display/defaultHeight");
	this->title = Config::get()->fetch<std::string>("Display/title");
	this->fullscreen = Config::get()->fetch<bool>("Display/fullscreen");
}

ym::Display* ym::Display::m_self = nullptr;

ym::Display* ym::Display::create(const DisplayDesc& description)
{
	if (m_self != nullptr) delete m_self;

#if YM_CURRENT_API_TYPE == YM_API_GL
		m_self = new GLDisplay(description);
#elif YM_CURRENT_API_TYPE == YM_API_DX11
		m_self = new DX11Display(description);
#endif

	return m_self;
}

ym::Display* ym::Display::get()
{
	return m_self;
}
