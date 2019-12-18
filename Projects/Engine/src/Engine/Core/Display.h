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
		bool vsync;

		DisplayDesc();
		DisplayDesc(const std::string& title) :
			width(0), height(0), title(title), fullscreen(true), vsync(false) {}
		DisplayDesc(int width, int height, const std::string& title) :
			width(width), height(height), title(title), fullscreen(false), vsync(false){}

		void init();
	};

	class Display
	{
	public:
		static Display* get();

		/*
			Creates the display. This is implemented in another class.
		*/
		static Display* create(const DisplayDesc& description);
		virtual ~Display() = default;

		virtual bool shouldClose() const noexcept = 0;
		virtual void close() noexcept = 0;

		virtual void pollEvents() noexcept = 0;

		virtual void swapBuffers() const noexcept = 0;

		virtual int getWidth() const noexcept = 0;
		virtual int getHeight() const noexcept = 0;

		virtual void* getNativeDisplay() = 0;

	private:
		static Display* m_self;
	};
};
