#include "DX11API.h"

#include "../../Engine/Core/Logger.h"
#include "../../Engine/Defines.h"

#include "../../Engine/Core/Display.h"

ym::DX11API* ym::DX11API::get()
{
	static DX11API api;
	return &api;
}

void ym::DX11API::init(DisplayDesc& displayDescriptor)
{
	int error;
	HRESULT result;

	// Create a DirectX graphics interface factory.
	IDXGIFactory* factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& factory);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the DirectX factory object!");

	// Create an adapter for the primary graphics interface (video card).
	IDXGIAdapter* adapter;
	// Select the first video card available.
	result = factory->EnumAdapters(0, &adapter);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the DirectX adapter object!");

	getRefreshRate(factory, adapter, displayDescriptor);

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to fetch adapter description from the video card!");

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	VideoCardInfo& videoCardInfo = getVideoCardInfo();
	// Store the dedicated video card memory in megabytes.
	videoCardInfo.videoMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	char name[128];
	unsigned long long len;
	error = wcstombs_s(&len, name, 128, adapterDesc.Description, 128);
	if (error != 0)
		YM_LOG_WARN("Failed to convert video card name from w-string to c-string!");
	videoCardInfo.name = std::string(name);

	// Release the factory.
	factory->Release();
	factory = 0;

	createSwapChainDeviceAndContext(displayDescriptor);

	// TODO: create the back buffer and depth buffer
	// TODO: Create depth stencil state
	// TODO: Create a rasterizer
	// TODO: Create the viewport

	// TODO: Create camera stuff.
}

void ym::DX11API::destroy()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, NULL);

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}

void ym::DX11API::getRefreshRate(IDXGIFactory* factory, IDXGIAdapter* adapter, DisplayDesc& displayDescriptor)
{
	HRESULT result;

	// Enumerate the primary adapter output (monitor).
	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create an DirectX adapter for the monitor!");

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to fetch the monitor's display mode list length!");

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	YM_ASSERT(displayModeList, "Could not initiate DirectX11: Failed to allocate memory for the monitor's display mode list!");

	// Fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to fetch the display mode list!");

	bool gotRefreshRate = false;
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)displayDescriptor.width)
		{
			if (displayModeList[i].Height == (unsigned int)displayDescriptor.height)
			{
				m_refreshRateNumerator = displayModeList[i].RefreshRate.Numerator;
				m_refreshRateDenominator = displayModeList[i].RefreshRate.Denominator;
				gotRefreshRate = true;
			}
		}
	}
	YM_ASSERT(gotRefreshRate, "Could not initiate DirectX11: Could not find a matching display mode for a with and height of ({0}, {1})!", displayDescriptor.width, displayDescriptor.height);

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;
}

void ym::DX11API::createSwapChainDeviceAndContext(DisplayDesc& displayDescriptor)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Single back buffer.
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = displayDescriptor.width;
	swapChainDesc.BufferDesc.Height = displayDescriptor.height;
	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	if (displayDescriptor.vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_refreshRateNumerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_refreshRateDenominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = (HWND)Display::get()->getNativeDisplay();
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (displayDescriptor.fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;
	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	YM_ASSERT(FAILED(result) == false, "Could not initiate DirectX11: Failed to create the swap chain, device and device context!");
}
