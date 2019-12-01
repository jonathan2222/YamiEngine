#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "../../Engine/Core/API.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <directxmath.h>

namespace ym
{
	struct DisplayDesc;
	class DX11API : public API
	{
	public:
		static DX11API* get();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;

	private:
		void getRefreshRate(IDXGIFactory* factory, IDXGIAdapter* adapter, DisplayDesc& displayDescriptor);

		unsigned int m_refreshRateNumerator;
		unsigned int m_refreshRateDenominator;
	};
}