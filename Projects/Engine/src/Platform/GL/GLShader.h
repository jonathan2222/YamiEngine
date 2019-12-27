#pragma once

#include "../../Engine/Core/Graphics/Shader.h"

namespace ym
{
	class GLShader : public Shader
	{
	public:
		GLShader();
		virtual ~GLShader();

		void load(const std::string& fileName, AttributeLayout& layout) override;
		void bind() override;
		void* getId() override;

	private:
	};
}