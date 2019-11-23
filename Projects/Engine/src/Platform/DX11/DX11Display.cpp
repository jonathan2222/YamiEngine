#include "DX11Display.h"

#include "../../Engine/Core/Logger.h"

ym::DX11Display* ym::DX11Display::globalDX11DisplayHandle = nullptr;

ym::DX11Display::DX11Display(const DisplayDesc& description) : m_window(nullptr), m_hinstance(nullptr), m_shouldClose(false)
{
	init(description);
}

ym::DX11Display::~DX11Display()
{
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
	globalDX11DisplayHandle = nullptr;
}

bool ym::DX11Display::shouldClose() const noexcept
{
	return m_shouldClose;
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

void ym::DX11Display::swapBuffers() const noexcept
{
	// TODO: Do this!
}

int ym::DX11Display::getWidth() const noexcept
{
	return m_description.width;
}

int ym::DX11Display::getHeight() const noexcept
{
	return m_description.height;
}

void* ym::DX11Display::getNativeDisplay()
{
	return m_window;
}

LRESULT ym::DX11Display::messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			// if key 'wparam' is pressed, send it to the input class!
			// TODO: Do this!
			return 0;
		}
		case WM_KEYUP:
		{
			// if key 'wparam' is released, send it to the input class!
			// TODO: Do this!
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
	m_description = description;

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
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
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;

		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = m_description.width;
		screenHeight = m_description.height;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		dwStyle = WS_OVERLAPPEDWINDOW;
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

LRESULT CALLBACK ym::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
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

		// All other messages pass to the message handler in the DX11Display class.
		default:
		{
			return ym::DX11Display::globalDX11DisplayHandle->messageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
