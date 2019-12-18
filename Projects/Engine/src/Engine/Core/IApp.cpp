#include "IApp.h"

#include "../Defines.h"
#include "Logger.h"
#include "Input/Config.h"

#include "API.h"
#include "Display.h"
#include "Input/Input.h"

ym::IApp::IApp(DisplayDesc& displayDescriptor) : m_display(nullptr), m_input(nullptr)
{
	YM_LOG_INIT();
	ym::Config::get()->init(YM_CONFIG_FILE_PATH);
	displayDescriptor.init();

	m_api = API::get();

	m_display = Display::create(displayDescriptor);
	m_api->init(displayDescriptor);
	API::VideoCardInfo& gpuInfo = m_api->getVideoCardInfo();
	YM_LOG_INFO("GPU: {0}", gpuInfo.name.c_str());

	m_input = ym::Input::create();
	m_input->init();
}

ym::IApp::~IApp()
{
	m_api->destroy();
	delete m_input;
}
