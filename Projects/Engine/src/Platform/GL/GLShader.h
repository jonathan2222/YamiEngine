#pragma once

#include "../../Engine/Core/Graphics/Shader.h"

namespace ym
{
	class GLShader : public Shader
	{
	public:
		GLShader();
		virtual ~GLShader();

		void load(const std::string& fileName, ShaderLayout layout) override;
		void bind() override;

	private:
	};
}