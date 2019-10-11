#pragma once

namespace ym
{
	class Display;
	class API;
	class IApp
	{
	public:
		IApp();
		virtual ~IApp();

		virtual void processArguments(int argc, char* argv[]) = 0;

		virtual void run() = 0;

	protected:
		ym::API* m_api;
		ym::Display* m_display;
	};
};