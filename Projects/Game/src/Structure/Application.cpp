#include "Application.h"

#include <Engine/Core/Display.h>

// Only for test, should be moved.
#ifdef YAMI_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
#include <gl/GL.h>

#include "Engine/Core/Input/Input.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Input/Config.h"
#include "Engine/Core/Graphics/Renderer.h"

Application::Application(ym::DisplayDesc& displayDescriptor) : IApp(displayDescriptor)
{
}

Application::~Application()
{
	delete m_display;
}

void Application::processArguments(int argc, char* argv[])
{	
}

void Application::run()
{
	while (!m_display->shouldClose())
	{
		m_display->pollEvents();

		if (ym::Input::get()->isKeyPressed(ym::Key::ESCAPE))
			m_display->close();

		if (ym::Input::get()->isKeyPressed(ym::Key::A))
			YM_LOG_INFO("Pressed A!");

		if (ym::Input::get()->isKeyPressed(ym::Key::LEFT_SHIFT))
			YM_LOG_INFO("Pressed SHIFT!");

		if (ym::Input::get()->isMBPressed(ym::MB::LEFT))
		{
			glm::vec2 pos = ym::Input::get()->getMousePos();
			YM_LOG_INFO("Mouse pos: {}, {}", pos.x, pos.y);
		}
		
		m_renderer->beginScene(1.0f, 0.0f, 0.0f, 1.0f);

		m_renderer->endScene();
	}
}
