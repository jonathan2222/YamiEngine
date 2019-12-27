#pragma once

#include"../../Engine/Core/Graphics/Renderer.h"
#include "DX11API.h"

namespace ym
{
	class DX11Renderer : public Renderer
	{
	public:
		static DX11Renderer* get();
		DX11Renderer();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;

		void beginScene(float r, float g, float b, float a) override;
		void endScene() override;

		void initShader(WCHAR* vertexShader, WCHAR* pixelShader) override;
		void bindShader(glm::mat4& world, glm::mat4& view, glm::mat4& proj) override;
		
		void draw(VertexArray* va, IndexBuffer* ib, Topology topology, Shader* shader) override;
		void draw(Model* model, Shader* shader) override;

	private:
		D3D11_PRIMITIVE_TOPOLOGY getD3D11Topology(Topology topology);

		void createRTV();
		void createDepthBuffer(DisplayDesc& displayDescriptor);
		void createDepthStencilState();
		void createDepthStencilView();
		void createRasterizer();
		void createAndSetViewport(DisplayDesc& displayDescriptor);

		void compileShader(ID3DBlob* errorMessageBlob, WCHAR* fileName);

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		IDXGISwapChain1* m_swapChain;

		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterizerState;

		ID3D11VertexShader* m_vsShader;
		ID3D11PixelShader* m_psShader;
		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_matrixBuffer;
	};
}
