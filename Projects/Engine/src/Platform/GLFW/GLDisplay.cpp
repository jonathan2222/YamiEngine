#include "GLDisplay.h"

#include <GLFW/glfw3.h>

ym::Display* ym::Display::create(const DisplayDesc& description)
{
	return new GLDisplay(description);
}

ym::GLDisplay::GLDisplay(const DisplayDesc& description) : m_window(nullptr)
{
	init(description);
}

ym::GLDisplay::~GLDisplay()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool ym::GLDisplay::shouldClose() const noexcept
{
	return glfwWindowShouldClose(m_window) != 0;
}

void ym::GLDisplay::pollEvents() const noexcept
{
	glfwPollEvents();
}

void ym::GLDisplay::swapBuffers() const noexcept
{
	glfwSwapBuffers(m_window);
}

int ym::GLDisplay::getWidth() const noexcept
{
	return m_description.width;
}

int ym::GLDisplay::getHeight() const noexcept
{
	return m_description.height;
}

void* ym::GLDisplay::getNativeDisplay()
{
	return m_window;
}

void ym::GLDisplay::init(const DisplayDesc& description)
{
	m_description = description;
	glfwInit();
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
}
