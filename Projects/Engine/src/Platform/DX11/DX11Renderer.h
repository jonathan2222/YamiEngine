#pragma once

#include"../../Engine/Core/Graphics/Renderer.h"
#include "DX11API.h"

namespace ym
{
	class DX11Renderer : public Renderer
	{
	public:
		static DX11Renderer* get();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;

		void beginScene(float r, float g, float b, float a) override;
		void endScene() override;

	private:
		void createRTV();
		void createDepthBuffer(DisplayDesc& displayDescriptor);
		void createDepthStencilState();
		void createDepthStencilView();
		void createRasterizer();
		void createAndSetViewport(DisplayDesc& displayDescriptor);

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		IDXGISwapChain1* m_swapChain;

		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterizerState;
	};
}
