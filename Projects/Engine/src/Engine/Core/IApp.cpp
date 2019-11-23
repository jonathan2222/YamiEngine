#include "IApp.h"

#include "../Defines.h"
#include "Logger.h"
#include "Input/Config.h"

#if YM_CURRENT_API_TYPE == YM_API_GL
	#include "../../Platform/GL/GLAPI.h"
#elif YM_CURRENT_API_TYPE == YM_API_DX11
	#include "../../Platform/DX11/DX11API.h"
#endif

#include "Input/Input.h"

ym::IApp::IApp() : m_display(nullptr), m_input(nullptr)
{
	YM_LOG_INIT();
	ym::Config::get()->init(YM_CONFIG_FILE_PATH);

#if YM_CURRENT_API_TYPE == YM_API_GL
	YM_LOG_INFO("Using API: OpenGL");
	m_api = GLAPI::get();
#elif YM_CURRENT_API_TYPE == YM_API_DX11
	YM_LOG_INFO("Using API: DirectX11");
	m_api = DX11API::get();
#endif

	m_api->init();

	m_input = ym::Input::create();
	m_input->init();
}

ym::IApp::~IApp()
{
	m_api->destroy();
	delete m_input;
}
