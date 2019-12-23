#pragma once

#include "../../Engine/Core/Graphics/Model.h"
#include "DX11API.h"

namespace ym
{
	class DX11Model : public Model
	{
	public:
		DX11Model();

		void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override;
		void bind() override;
		void draw() override;
		void destroy() override;

		unsigned int getVertexCount() override;
		unsigned int getIndexCount() override;
	private:
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		unsigned int m_indexCount;
		unsigned int m_vertexCount;
	};
}