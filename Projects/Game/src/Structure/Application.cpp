#include "Application.h"

#include <Engine/Core/Display.h>

// Only for test, should be moved.
#ifdef YAMI_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
#include <gl/GL.h>

#include "Engine/Core/Input/Input.h"
#include "Engine/Core/Logger.h"

Application::Application()
{
	// TODO: Make input independent from display.
	this->m_display = ym::Display::create(ym::DisplayDesc{});
	ym::Input::get()->init();
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
		// TODO: No platform specific functions in api!!
		m_display->pollEvents();

		if (ym::Input::get()->isKeyPressed(ym::Key::A))
			YM_LOG_INFO("Pressed A!");

		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glEnd();

		m_display->swapBuffers();
	}
}
