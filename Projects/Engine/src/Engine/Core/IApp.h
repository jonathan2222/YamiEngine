#pragma once

namespace ym
{
	class Display;
	class IApp
	{
	public:
		IApp() : m_display(nullptr) {};
		virtual ~IApp() = default;

		virtual void processArguments(int argc, char* argv[]) = 0;

		virtual void run() = 0;

	protected:
		ym::Display* m_display;
	};
};