#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "../../Engine/Core/API.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

#ifdef YAMI_DEBUG
#include "comdef.h"
#define YM_DX11_ASSERT_CHECK(hr, msg, ...) \
	{ \
		_com_error err(HRESULT_CODE(hr)); \
		const char* errMsg = (const char*)err.ErrorMessage(); \
		const unsigned int SIZE = 512; \
		char str[SIZE]; \
		strcpy_s(str, "[Error: "); \
		strcat_s(str, SIZE, errMsg); \
		strcat_s(str, SIZE, "  (0x{0:x})] "); \
		strcat_s(str, SIZE, msg); \
		YM_ASSERT(SUCCEEDED(hr), str, (unsigned int)hr, __VA_ARGS__); \
	}
#else
#define YM_DX11_ASSERT_CHECK(hr, msg, ...) YM_ASSERT(SUCCEEDED(hr), msg, __VA_ARGS__);
#endif

#include <vector>
#include "../../Engine/Core/Graphics/Types.h"

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

		DXGI_FORMAT convertFormat(Format format) const;
		D3D11_FILTER convertFilter(Sampler::Filter filter) const;
		D3D11_TEXTURE_ADDRESS_MODE convertSamplerAddressMode(Sampler::AddressMode addressMode) const;

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

		#ifdef YAMI_DEBUG
		ID3D11Debug* m_debug;
		#endif
	};
}