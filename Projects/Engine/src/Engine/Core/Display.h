#pragma once

#include <string>

struct GLFWwindow;

namespace ym
{
	struct DisplayDesc
	{
		std::string title;
		int width;
		int height;
		bool fullscreen;

		DisplayDesc(const std::string& title) :
			width(0), height(0), title(title), fullscreen(true) {}
		DisplayDesc(int width = 1280, int height = 720, const std::string& title = "Yami Window") :
			width(width), height(height), title(title), fullscreen(false) {}

	};

	class Display
	{
	public:
		static Display* get();

		/*
			Creates the display. This is implemented in another class.
		*/
		static Display* create(const DisplayDesc& description = DisplayDesc());
		virtual ~Display() = default;

		virtual bool shouldClose() const noexcept = 0;

		virtual void pollEvents() const noexcept = 0;

		virtual void swapBuffers() const noexcept = 0;

		virtual int getWidth() const noexcept = 0;
		virtual int getHeight() const noexcept = 0;

		virtual void* getNativeDisplay() = 0;

	private:
		static Display* m_self;
	};
};
