#include "stdafx.h"
#include "GLDisplay.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLAPI.h"

#include "../../Engine/Core/Graphics/Renderer.h"

ym::GLDisplay* ym::GLDisplay::g_glDisplayPtr = nullptr;

ym::GLDisplay::GLDisplay(const DisplayDesc& description) : m_window(nullptr), m_shouldClose(false)
{
	YM_PROFILER_FUNCTION();
	init(description);
}

ym::GLDisplay::~GLDisplay()
{
	glfwDestroyWindow(m_window);
}

bool ym::GLDisplay::shouldClose() const noexcept
{
	return glfwWindowShouldClose(m_window) != 0 || m_shouldClose;
}

void ym::GLDisplay::close() noexcept
{
	m_shouldClose = true;
}

void ym::GLDisplay::pollEvents() noexcept
{
	glfwPollEvents();
}

void* ym::GLDisplay::getNativeDisplay()
{
	return m_window;
}

void ym::GLDisplay::init(const DisplayDesc& description)
{
	YM_PROFILER_FUNCTION();

	g_glDisplayPtr = this;
	m_description = description;
	m_shouldClose = false;
	
	GLFWmonitor* monitor = nullptr;
	if (m_description.fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		m_description.width = mode->width;
		m_description.height = mode->height;
	}

	m_window = glfwCreateWindow(m_description.width, m_description.height, m_description.title.c_str(), monitor, nullptr);
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, GLDisplay::frameBufferSizeCallback);

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		YM_ASSERT(false, "Failed to initialize GLEW!");
	}

	GLAPI::get()->initDebug();

	// Enable VSync if set.
	if(m_description.vsync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void ym::GLDisplay::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	YM_PROFILER_RENDERING_FUNCTION();
	// Resize buffers.
	if (Renderer::get()->isActive())
		Renderer::get()->resize(width, height);

	// Update display description.
	DisplayDesc& description = g_glDisplayPtr->getDescription();
	description.width = width;
	description.width = height;
	description.fullscreen = false;
}
