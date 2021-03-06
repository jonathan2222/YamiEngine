#pragma once

#include "../../Engine/Core/Display.h"

#include "DX11API.h"

namespace ym
{
	class DX11Display : public Display
	{
	public:
		DX11Display(const DisplayDesc& description);
		virtual ~DX11Display();

		bool shouldClose() const noexcept override;
		void close() noexcept override;

		void pollEvents() noexcept override;

		void* getNativeDisplay() override;

		LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);
		static DX11Display* g_DX11DisplayHandle;
	private:
		void init(const DisplayDesc& description);

		std::wstring m_appNameWS;
		HWND m_window;
		HINSTANCE m_hinstance;

		MSG m_msg;
		bool m_shouldClose;
	};

	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
}

