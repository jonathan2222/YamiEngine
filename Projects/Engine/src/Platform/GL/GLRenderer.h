#pragma once

#include"../../Engine/Core/Graphics/Renderer.h"

namespace ym
{
	class GLRenderer : public Renderer
	{
	public:
		static GLRenderer* get();
		GLRenderer();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;

		void beginScene(float r, float g, float b, float a) override;
		void endScene() override;

		void draw(VertexArray* va, IndexBuffer* ib, Topology topology, Shader* shader) override;
		void draw(Model* model, Shader* shader) override;
	};
}