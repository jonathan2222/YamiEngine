#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "../../Engine/Core/API.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <vector>

namespace ym
{
	struct DisplayDesc;
	class DX11API : public API
	{
	public:
		static DX11API* get();

		void preDisplayInit(DisplayDesc& displayDescriptor) override;
		void postDisplayInit() override;
		void destroy() override;

		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();
		IDXGISwapChain1* getSwapChain();

	private:
		void createDevice(IDXGIAdapter* adapter, D3D_DRIVER_TYPE driverType);
		void createSwapChain();
		void getRefreshRate(IDXGIAdapter* adapter, DisplayDesc& displayDescriptor);
		std::vector<IDXGIAdapter*> enumerateAdapters();

		void fillVideoCardInfo(IDXGIAdapter* adapter);

		IDXGIFactory2* m_factory;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		IDXGISwapChain1* m_swapChain;
	};
}