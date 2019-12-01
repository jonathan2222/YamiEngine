#include "IApp.h"

#include "../Defines.h"
#include "Logger.h"
#include "Input/Config.h"

#include "API.h"

#include "Input/Input.h"

ym::IApp::IApp() : m_display(nullptr), m_input(nullptr)
{
	YM_LOG_INIT();
	ym::Config::get()->init(YM_CONFIG_FILE_PATH);

	m_api = API::get();

	m_api->init();

	m_input = ym::Input::create();
	m_input->init();
}

ym::IApp::~IApp()
{
	m_api->destroy();
	delete m_input;
}
