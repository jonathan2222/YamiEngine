#pragma once

#include "../../Engine/Core/Graphics/Model.h"

namespace ym
{
	class GLModel : public Model
	{
	public:
		GLModel();

		void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override;
		void bind() override;
		void draw() override;
		void destroy() override;

		unsigned int getVertexCount() override;
		unsigned int getIndexCount() override;
	private:
		unsigned int m_vertexCount;
		unsigned int m_indexCount;
	};
}
