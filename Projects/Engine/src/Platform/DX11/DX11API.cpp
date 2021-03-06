#include "stdafx.h"
#include "DX11API.h"

#include "DX11Display.h"

ym::DX11API* ym::DX11API::get()
{
	static DX11API api;
	return &api;
}

void ym::DX11API::preDisplayInit(DisplayDesc& displayDescriptor)
{
	YM_PROFILER_FUNCTION();
	HRESULT result;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory2), (void**)& m_factory);
	YM_DX11_ASSERT_CHECK(result, "Could not initiate DirectX11: Failed to create the DirectX factory object!");

	// Go through graphics adapters which are compatible with DirectX.
	IDXGIAdapter* adapter = nullptr;
	std::vector <IDXGIAdapter*> adapters = enumerateAdapters();
	adapter = adapters[0];
	/*DXGI_ADAPTER_DESC descI;
	DXGI_ADAPTER_DESC descCurrent;
	for (IDXGIAdapter*& ad : adapters)
	{
		ZeroMemory(&descI, sizeof(descI));
		ad->GetDesc(&descI);

		if (adapter == nullptr) adapter = ad;
		else
		{
			ZeroMemory(&descCurrent, sizeof(descCurrent));
			adapter->GetDesc(&descCurrent);
			// TODO: Change this, this is a bad way of fetching the gpu.
			if (descI.DedicatedVideoMemory > descCurrent.DedicatedVideoMemory)
			{
				adapter = ad;
			}
		}
	}
	*/
	fillVideoCardInfo(adapter);

	createDevice(adapter, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN);
	
	// Fetch refresh rate from the default adapter.
	getRefreshRate(adapters[0], displayDescriptor);

	// Release the adapters.
	UINT len = (UINT)adapters.size();
	for (auto& ad : adapters)
		ad->Release();
	adapter = 0;
}

void ym::DX11API::postDisplayInit()
{
	YM_PROFILER_FUNCTION();
	createSwapChain();
	// Release the factory.
	m_factory->Release();
	m_factory = 0;
}

void ym::DX11API::destroy()
{
	YM_PROFILER_FUNCTION();
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, NULL);

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}
	
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	if (m_device)
	{
#ifdef YAMI_DEBUG
		static bool shouldDebug = Config::get()->fetch<bool>("API/debug");
		if (shouldDebug)
		{
			m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug));
			m_debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_SUMMARY);
			m_debug->Release();
		}
#endif
		m_device->Release();
		m_device = 0;
	}

}

ID3D11Device* ym::DX11API::getDevice()
{
	return m_device;
}

ID3D11DeviceContext* ym::DX11API::getDeviceContext()
{
	return m_deviceContext;
}

IDXGISwapChain1* ym::DX11API::getSwapChain()
{
	return m_swapChain;
}

DXGI_FORMAT ym::DX11API::convertFormat(Format format) const
{
	switch (format)
	{
	case Format::FLOAT_32_R:		return DXGI_FORMAT_R32_FLOAT;
	case Format::FLOAT_32_RG:		return DXGI_FORMAT_R32G32_FLOAT;
	case Format::FLOAT_32_RGB:		return DXGI_FORMAT_R32G32B32_FLOAT;
	case Format::SINT_32_R:			return DXGI_FORMAT_R32_SINT;
	case Format::SINT_32_RG:		return DXGI_FORMAT_R32G32_SINT;
	case Format::SINT_32_RGB:		return DXGI_FORMAT_R32G32B32_SINT;
	case Format::SINT_32_RGBA:		return DXGI_FORMAT_R32G32B32A32_SINT;
	case Format::UINT_32_R:			return DXGI_FORMAT_R32_UINT;
	case Format::UINT_32_RG:		return DXGI_FORMAT_R32G32_UINT;
	case Format::UINT_32_RGB:		return DXGI_FORMAT_R32G32B32_UINT;
	case Format::UINT_32_RGBA:		return DXGI_FORMAT_R32G32B32A32_UINT;
	case Format::SINT_8_R:			return DXGI_FORMAT_R8_SINT;
	case Format::SINT_8_RG:			return DXGI_FORMAT_R8G8_SINT;
	case Format::SINT_8_RGB:		// Assume it should be SINT_8_RGBA, (DirectX does not support this)
	case Format::SINT_8_RGBA:		return DXGI_FORMAT_R8G8B8A8_SINT;
	case Format::UINT_8_R:			return DXGI_FORMAT_R8_UINT;
	case Format::UINT_8_RG:			return DXGI_FORMAT_R8G8_UINT;
	case Format::UINT_8_RGB:		// Assume it should be UINT_8_RGBA, (DirectX does not support this)
	case Format::UINT_8_RGBA:		return DXGI_FORMAT_R8G8B8A8_UINT;
	case Format::FLOAT_32_RGBA:
	default:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
}

D3D11_FILTER ym::DX11API::convertFilter(Sampler::Filter filter) const
{
	switch (filter)
	{
	case Sampler::Filter::MIN_NEAREST_MAG_NEAREST_MIP_NEAREST:		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case Sampler::Filter::MIN_LINEAR_MAG_NEAREST_MIP_NEAREST:		return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	case Sampler::Filter::MIN_NEAREST_MAG_LINEAR_MIP_NEAREST:		return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	case Sampler::Filter::MIN_LINEAR_MAG_LINEAR_MIP_NEAREST:		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	case Sampler::Filter::MIN_NEAREST_MAG_NEAREST_MIP_LINEAR:		return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	case Sampler::Filter::MIN_LINEAR_MAG_NEAREST_MIP_LINEAR:		return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	case Sampler::Filter::MIN_NEAREST_MAG_LINEAR_MIP_LINEAR:		return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	case Sampler::Filter::MIN_LINEAR_MAG_LINEAR_MIP_LINEAR:
	default:
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
}

D3D11_TEXTURE_ADDRESS_MODE ym::DX11API::convertSamplerAddressMode(Sampler::AddressMode addressMode) const
{
	switch (addressMode)
	{
	case Sampler::AddressMode::CLAMP_TO_EDGE:		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case Sampler::AddressMode::MIRRORED_REPEAT:		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case Sampler::AddressMode::REPEAT:
	default:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}

void ym::DX11API::createDevice(IDXGIAdapter* adapter, D3D_DRIVER_TYPE driverType)
{
	YM_PROFILER_FUNCTION();
	UINT flags = 0;
#ifdef YAMI_DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// Set the feature level to DirectX 11.1.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	// Create a D3D11Device and a device context.
	HRESULT result = D3D11CreateDevice(adapter, driverType, NULL, flags,
		&featureLevel, 1, D3D11_SDK_VERSION, &m_device, NULL, &m_deviceContext);
	YM_DX11_ASSERT_CHECK(result, "Could not initiate DirectX11: Failed to create the device and device context!");
}

void ym::DX11API::createSwapChain()
{
	YM_PROFILER_FUNCTION();
	DisplayDesc displayDiscription = DX11Display::get()->getDescription();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.Width = (UINT)displayDiscription.width;
	swapChainDesc.Height = (UINT)displayDiscription.height;
	// Double back buffer.
	swapChainDesc.BufferCount = 2;
	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
	ZeroMemory(&fullscreenDesc, sizeof(fullscreenDesc));
	fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fullscreenDesc.Windowed = displayDiscription.fullscreen ? FALSE : TRUE;
	if (displayDiscription.vsync)
	{
		fullscreenDesc.RefreshRate.Numerator = displayDiscription.refreshRateNumerator;
		fullscreenDesc.RefreshRate.Denominator = displayDiscription.refreshRateDenominator;
	}
	else
	{
		fullscreenDesc.RefreshRate.Numerator = 0;
		fullscreenDesc.RefreshRate.Denominator = 1;
	}

	HWND wnd = (HWND)DX11Display::get()->getNativeDisplay();
	HRESULT result = m_factory->CreateSwapChainForHwnd(m_device, wnd, &swapChainDesc, &fullscreenDesc, NULL, &m_swapChain);
	YM_DX11_ASSERT_CHECK(result, "Could not initiate DirectX11: Failed to create swap chain!");
}


void ym::DX11API::getRefreshRate(IDXGIAdapter* adapter, DisplayDesc& displayDescriptor)
{
	YM_PROFILER_FUNCTION();
	HRESULT result;

	// Enumerate the primary adapter output (monitor).
	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	YM_DX11_ASSERT_CHECK(result, "Could not initiate DirectX11: Failed to create an DirectX adapter for the monitor!");

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	YM_DX11_ASSERT_CHECK(result, "Could not initiate DirectX11: Failed to fetch the monitor's display mode list length!");

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	YM_ASSERT(displayModeList, "Could not initiate DirectX11: Failed to allocate memory for the monitor's display mode list!");

	// Fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	YM_DX11_ASSERT_CHECK(result, "Could not initiate DirectX11: Failed to fetch the display mode list!");

	bool gotRefreshRate = false;
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)displayDescriptor.width)
		{
			if (displayModeList[i].Height == (unsigned int)displayDescriptor.height)
			{
				displayDescriptor.refreshRateNumerator = displayModeList[i].RefreshRate.Numerator;
				displayDescriptor.refreshRateDenominator = displayModeList[i].RefreshRate.Denominator;
				gotRefreshRate = true;
				break;
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

std::vector<IDXGIAdapter*> ym::DX11API::enumerateAdapters()
{
	YM_PROFILER_FUNCTION();
	IDXGIAdapter* pAdapter;
	std::vector <IDXGIAdapter*> vAdapters;

	for (unsigned int i = 0; m_factory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		vAdapters.push_back(pAdapter);

	return vAdapters;
}

void ym::DX11API::fillVideoCardInfo(IDXGIAdapter* adapter)
{
	YM_PROFILER_FUNCTION();

	HRESULT result;
	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	YM_DX11_ASSERT_CHECK(result, "Could not initiate DirectX11: Failed to fetch adapter description from the video card!");

	VideoCardInfo& videoCardInfo = getVideoCardInfo();
	// Store the dedicated video card memory in megabytes.
	videoCardInfo.videoMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	char name[128];
	unsigned long long len;
	int error = wcstombs_s(&len, name, 128, adapterDesc.Description, 128);
	if (error != 0)
		YM_LOG_WARN("Failed to convert video card name from w-string to c-string!");
	videoCardInfo.name = std::string(name);

#ifdef YAMI_DEBUG
	YM_LOG_INFO("----- Adapter -----");
	YM_LOG_INFO("Description: {0}", name);
	YM_LOG_INFO("VendorId: {0}", adapterDesc.VendorId);
	YM_LOG_INFO("DeviceId: {0}", adapterDesc.DeviceId);
	YM_LOG_INFO("SubSysId: {0}", adapterDesc.SubSysId);
	YM_LOG_INFO("Revision: {0}", adapterDesc.Revision);
	YM_LOG_INFO("DedicatedVideoMemory: {0}", adapterDesc.DedicatedVideoMemory);
	YM_LOG_INFO("DedicatedSystemMemory: {0}", adapterDesc.DedicatedSystemMemory);
	YM_LOG_INFO("SharedSystemMemory: {0}", adapterDesc.SharedSystemMemory);
#endif
}
