#include "Input.h"

#include "../../Defines.h"
#if YM_CURRENT_API_TYPE == YM_API_GL
	#include "../../../Platform/DX11/DX11Input.h"
#elif YM_CURRENT_API_TYPE == YM_API_DX11
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

#if YM_CURRENT_API_TYPE == YM_API_GL
	m_self = new DX11Input();
#elif YM_CURRENT_API_TYPE == YM_API_DX11
	m_self = new DX11Input();
#endif

	return m_self;
}