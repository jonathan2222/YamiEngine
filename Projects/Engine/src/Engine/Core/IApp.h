#pragma once

namespace ym
{
	class Renderer;
	struct DisplayDesc;
	class Display;
	class Input;
	class API;
	class IApp
	{
	public:
		IApp(DisplayDesc& displayDescriptor);
		virtual ~IApp();

		virtual void processArguments(int argc, char* argv[]) = 0;

		virtual void run() = 0;

	protected:
		ym::API* m_api;
		ym::Input* m_input;
		ym::Display* m_display;
		ym::Renderer* m_renderer;
	};
};