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
		void createSwapChainDeviceAndContext(DisplayDesc& displayDescriptor);
		void createRTV();
		void createDepthBuffer(DisplayDesc& displayDescriptor);
		void createDepthStencilState();
		void createDepthStencilView();
		void createRasterizer();
		void createAndSetViewport(DisplayDesc& displayDescriptor);

		unsigned int m_refreshRateNumerator;
		unsigned int m_refreshRateDenominator;
		IDXGISwapChain* m_swapChain;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterizerState;
	};
}