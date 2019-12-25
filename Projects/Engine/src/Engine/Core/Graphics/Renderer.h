#pragma once

#include <vector>
#include <glm/mat4x4.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"

namespace ym
{
	struct DisplayDesc;
	class Renderer
	{
	public:
		static Renderer* get();

		virtual void init(DisplayDesc& displayDescriptor) = 0;
		virtual void destroy() = 0;

		virtual void beginScene(float r, float g, float b, float a) = 0;
		virtual void endScene() = 0;

		virtual void initShader(WCHAR* vertexShader, WCHAR* pixelShader) = 0;
		virtual void bindShader(glm::mat4& world, glm::mat4& view, glm::mat4& proj) = 0;

		virtual void draw(VertexArray* va, IndexBuffer* ib, Topology topology) = 0;
		virtual void draw(Model* model) = 0;
	};
}