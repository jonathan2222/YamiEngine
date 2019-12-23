#pragma once

#include"../../Engine/Core/Graphics/Renderer.h"

namespace ym
{
	class GLRenderer : public Renderer
	{
	public:
		static GLRenderer* get();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;

		void beginScene(float r, float g, float b, float a) override;
		void endScene() override;

		void initShader(WCHAR* vertexShader, WCHAR* pixelShader) override;
		void bindShader(glm::mat4& world, glm::mat4& view, glm::mat4& proj) override;
	};
}