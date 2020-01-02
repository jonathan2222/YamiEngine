#include "stdafx.h"
#include "DX11Display.h"

#include "DX11Input.h"
#include "DX11Renderer.h"
// For ImGui::GetIO().
#include "DX11ImGuiImpl.h"

ym::DX11Display* ym::DX11Display::g_DX11DisplayHandle = nullptr;

ym::DX11Display::DX11Display(const DisplayDesc& description) : m_window(nullptr), m_hinstance(nullptr), m_shouldClose(false)
{
	YM_PROFILER_FUNCTION();
	init(description);
}

ym::DX11Display::~DX11Display()
{
	YM_PROFILER_FUNCTION();
	// Fix the display settings if leaving full screen mode.
	if (m_description.fullscreen)
		ChangeDisplaySettings(NULL, 0);

	// Remove the window.
	DestroyWindow(m_window);
	m_window = nullptr;

	// Remove the application instance.
	UnregisterClass(m_appNameWS.c_str(), m_hinstance);
	m_hinstance = nullptr;

	// Release the pointer to this class.
	g_DX11DisplayHandle = nullptr;
}

bool ym::DX11Display::shouldClose() const noexcept
{
	return m_shouldClose;
}

void ym::DX11Display::close() noexcept
{
	m_shouldClose = true;
}

void ym::DX11Display::pollEvents() noexcept
{
	if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}

	if (m_msg.message == WM_QUIT)
		m_shouldClose = true;
}

void* ym::DX11Display::getNativeDisplay()
{
	return m_window;
}

LRESULT ym::DX11Display::messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	YM_PROFILER_RENDERING_FUNCTION();
	// TODO: All keys are not accounted for, like insert, delete, caps-lock, and so on. Make this work!
	DX11Input* input = dynamic_cast<DX11Input*>(DX11Input::get());
	int keycode = (int)wparam;
	switch (umsg)
	{
		case WM_LBUTTONDOWN: { input->setMb(MB::LEFT, KeyState::PRESSED); return 0; }
		case WM_LBUTTONUP: { input->setMb(MB::LEFT, KeyState::RELEASED); return 0; }
		case WM_RBUTTONDOWN: { input->setMb(MB::RIGHT, KeyState::PRESSED); return 0; }
		case WM_RBUTTONUP: { input->setMb(MB::RIGHT, KeyState::RELEASED); return 0; }
		case WM_MBUTTONDOWN: { input->setMb(MB::MIDDLE, KeyState::PRESSED); return 0; }
		case WM_MBUTTONUP: { input->setMb(MB::MIDDLE, KeyState::RELEASED); return 0; }
		case WM_KEYDOWN:
		{
			// if key 'wparam' is pressed, send it to the input class!
			if (keycode == VK_SHIFT)
			{
				if (GetKeyState(VK_LSHIFT) & 0x8000) keycode = (int)KeyConverter::DXKey::LEFT_SHIFT;
				if (GetKeyState(VK_RSHIFT) & 0x8000) keycode = (int)KeyConverter::DXKey::RIGHT_SHIFT;
			}
			else if (keycode == VK_CONTROL)
			{
				if (GetKeyState(VK_LCONTROL) & 0x8000) keycode = (int)KeyConverter::DXKey::LEFT_CONTROL;
				if (GetKeyState(VK_RCONTROL) & 0x8000) keycode = (int)KeyConverter::DXKey::RIGHT_CONTROL;
			}
			else if (keycode == VK_MENU)
			{
				if (GetKeyState(VK_LMENU) & 0x8000) keycode = (int)KeyConverter::DXKey::LEFT_ALT;
				if (GetKeyState(VK_RMENU) & 0x8000) keycode = (int)KeyConverter::DXKey::RIGHT_ALT;
			}
			input->setKey(KeyConverter::convertToKey((KeyConverter::DXKey)keycode), KeyState::PRESSED);
			return 0;
		}
		case WM_KEYUP:
		{
			// if key 'wparam' is released, send it to the input class!
			if (keycode == VK_SHIFT)
			{
				if (input->getKeyState(Key::LEFT_SHIFT) == ym::KeyState::PRESSED)
					if ((GetKeyState(VK_LSHIFT) & 0x8000) == 0) keycode = (int)KeyConverter::DXKey::LEFT_SHIFT;
				if (input->getKeyState(Key::RIGHT_SHIFT) == ym::KeyState::PRESSED)
					if ((GetKeyState(VK_RSHIFT) & 0x8000) == 0) keycode = (int)KeyConverter::DXKey::RIGHT_SHIFT;
			}
			else if (keycode == VK_CONTROL)
			{
				if (input->getKeyState(Key::LEFT_CONTROL) == ym::KeyState::PRESSED)
					if ((GetKeyState(VK_LCONTROL) & 0x8000) == 0) keycode = (int)KeyConverter::DXKey::LEFT_CONTROL;
				if (input->getKeyState(Key::RIGHT_CONTROL) == ym::KeyState::PRESSED)
					if ((GetKeyState(VK_RCONTROL) & 0x8000) == 0) keycode = (int)KeyConverter::DXKey::RIGHT_CONTROL;
			}
			else if (keycode == VK_MENU)
			{
				if (input->getKeyState(Key::LEFT_ALT) == ym::KeyState::PRESSED)
					if ((GetKeyState(VK_LMENU) & 0x8000) == 0) keycode = (int)KeyConverter::DXKey::LEFT_ALT;
				if (input->getKeyState(Key::RIGHT_ALT) == ym::KeyState::PRESSED)
					if ((GetKeyState(VK_RMENU) & 0x8000) == 0) keycode = (int)KeyConverter::DXKey::RIGHT_ALT;
			}
			input->setKey(KeyConverter::convertToKey((KeyConverter::DXKey)keycode), KeyState::RELEASED);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lparam);
			int yPos = HIWORD(lparam);
			input->setMousePos(xPos, yPos);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void ym::DX11Display::init(const DisplayDesc& description)
{
	YM_PROFILER_FUNCTION();
	setDescription(description);
	ym::DX11Display::g_DX11DisplayHandle = this;

	int posX, posY;

	m_hinstance = GetModuleHandle(NULL);

	auto s2ws = [&](const std::string & s) -> std::wstring
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	};

	m_appNameWS = s2ws(m_description.title);

	// Setup the windows class with default settings.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = ym::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appNameWS.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	DWORD dwStyle;

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_description.fullscreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32; // Color resolution, in bits per pixel.
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;

		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	}
	else
	{
		screenWidth = m_description.width;
		screenHeight = m_description.height;

		// Adjust the size, so that the drawing area becomes the m_description.width and m_description.height!.
		dwStyle = WS_OVERLAPPEDWINDOW;
		RECT wr = { 0, 0, screenWidth, screenHeight };
		AdjustWindowRect(&wr, dwStyle, FALSE);
		// Get the new size of the window.
		screenWidth = wr.right - wr.left;
		screenHeight = wr.bottom - wr.top;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_window = CreateWindowEx(WS_EX_APPWINDOW, m_appNameWS.c_str(), m_appNameWS.c_str(), dwStyle,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_window, SW_SHOW);
	SetForegroundWindow(m_window);
	SetFocus(m_window);

	ZeroMemory(&m_msg, sizeof(MSG));

	// Hide the mouse cursor.
	//ShowCursor(false);
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ym::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;
		
	switch (umessage)
	{
		case WM_SIZE:
		{
			if (Renderer::get()->isActive() && wparam != SIZE_MINIMIZED)
				Renderer::get()->resize((unsigned int)LOWORD(lparam), (unsigned int)HIWORD(lparam));
			return 0;
		}

		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		/*
		case WM_SYSCOMMAND:
		{
			// Disable ALT application menu
			if ((wparam & 0xfff0) == SC_KEYMENU)
				return 0;
			break;
		}*/
		
		case WM_DPICHANGED:
		{
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
			{
				//const int dpi = HIWORD(wparam);
				//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
				const RECT* suggested_rect = (RECT*)lparam;
				::SetWindowPos(hwnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
			}
			return 0;
		}

		// All other messages pass to the message handler in the DX11Display class.
		default:
		{
			return ym::DX11Display::g_DX11DisplayHandle->messageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
