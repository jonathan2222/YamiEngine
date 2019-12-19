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
	int error = wcstombs_s(&len, name, 128, adapterDesc.Description, 128);
	if (error != 0)
		YM_LOG_WARN("Failed to convert video card name from w-string to c-string!");
	videoCardInfo.name = std::string(name);

	// Release the factory.
	factory->Release();
	factory = 0;
}

void ym::DX11API::destroy()
{

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
