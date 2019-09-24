#include "Display.h"

#include <GLFW/glfw3.h>

namespace ym
{
	Display::Display(int width, int height, const std::string& title)
	{
		glfwInit();
		this->wnd = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(wnd);
	}

	Display::~Display()
	{
		glfwDestroyWindow(wnd);
		glfwTerminate();
	}

	bool Display::shouldClose() const noexcept
	{
		return glfwWindowShouldClose(wnd) != 0;
	}

	void Display::pollEvents() const noexcept
	{
		glfwPollEvents();
	}

	void Display::swapBuffers() const noexcept
	{
		glfwSwapBuffers(wnd);
	}

	std::pair<int, int> Display::getWindowSize() const noexcept
	{
		std::pair<int, int> size{};
		glfwGetWindowSize(wnd, &size.first, &size.second);
		return size;
	}
}
