#pragma once

#include <string>
#include <memory>

struct GLFWwindow;

namespace ym
{
	class Display
	{
	public:
		Display(int width, int height, const std::string& title);
		~Display();

		bool shouldClose() const noexcept;

		void pollEvents() const noexcept;

		void swapBuffers() const noexcept;

		std::pair<int, int> getWindowSize() const noexcept;
	
	private:
		GLFWwindow* wnd;
	};
};
