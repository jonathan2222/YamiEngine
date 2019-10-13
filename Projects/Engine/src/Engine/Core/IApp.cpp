#include "IApp.h"

#include "../../Platform/GL/GLAPI.h"
#include "Logger.h"

ym::IApp::IApp() : m_display(nullptr)
{
	YM_LOG_INIT();
	m_api = GLAPI::get();

	m_api->init();
}

ym::IApp::~IApp()
{
	m_api->destroy();
}
