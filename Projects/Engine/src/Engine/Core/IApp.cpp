#include "stdafx.h"
#include "IApp.h"

#include "API.h"
#include "ImGuiImpl.h"
#include "Display.h"
#include "Input/Input.h"
#include "Graphics/Renderer.h"
#include "LayerManager.h"

#include "Utils/Timer.h"

ym::IApp::IApp(DisplayDesc& displayDescriptor) : m_display(nullptr), m_input(nullptr)
{
	YM_LOG_INIT();
	ym::Config::get()->init(YM_CONFIG_FILE_PATH);
	displayDescriptor.init();
	std::string apiType = Config::get()->fetch<std::string>("API/type");
	YM_LOG_INFO("Using API: {0}", apiType.c_str());
	
	KeyConverter::init();

	m_api = API::get();
	m_api->preDisplayInit(displayDescriptor);
	m_display = Display::create(displayDescriptor);
	m_api->postDisplayInit();

	m_renderer = Renderer::get();
	m_renderer->init(displayDescriptor);

	API::VideoCardInfo& gpuInfo = m_api->getVideoCardInfo();
	YM_LOG_INFO("GPU: {0}", gpuInfo.name.c_str());

	m_input = ym::Input::create();
	m_input->init();

	m_layerManager = LayerManager::get();
	m_layerManager->setApp(this);

	m_imGuiImpl = ImGuiImpl::create();
	m_imGuiImpl->setUp();
}

ym::IApp::~IApp()
{
	m_imGuiImpl->cleanUp();
	delete m_imGuiImpl;
	m_renderer->destroy();

	delete m_display;
	m_api->destroy();
	delete m_input;
}

void ym::IApp::run()
{
	start();
	m_layerManager->onStart();

	ym::Timer timer;
	float dt = 0.16f;
	float debugTimer = 1.0f;
	while (!m_display->shouldClose())
	{
		timer.start();
		m_display->pollEvents();

		m_layerManager->onUpdate(dt);

		m_renderer->beginScene(0.0f, 0.0f, 0.0f, 1.0f);
		
		m_layerManager->onRender();

		m_imGuiImpl->startFrame();
		m_layerManager->onRenderImGui();
		m_imGuiImpl->endFrame();

		m_renderer->endScene();

		dt = timer.stop();
		debugTimer += dt;
		if (debugTimer >= 1.0f)
		{
			debugTimer = 0.0f;
			YM_LOG_INFO("dt: {0:.2f} ms (FPS: {1:.2f})", dt * 1000.f, 1.0f / dt);
		}
	}

	m_layerManager->onQuit();
}

ym::LayerManager* ym::IApp::getLayerManager()
{
	return m_layerManager;
}
