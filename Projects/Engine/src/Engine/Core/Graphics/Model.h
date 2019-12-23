#pragma once

#include <vector>
#include "Vertex.h"

namespace ym
{
	class Model
	{
	public:
		static Model* create();

		virtual void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) = 0;
		virtual void bind() = 0;
		virtual void draw() = 0;
		virtual void destroy() = 0;

		virtual unsigned int getVertexCount() = 0;
		virtual unsigned int getIndexCount() = 0;
	};
}