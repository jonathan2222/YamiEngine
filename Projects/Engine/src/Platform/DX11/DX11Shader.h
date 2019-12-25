#pragma once

#include "../../Engine/Core/Graphics/Shader.h"

#include "DX11API.h"

namespace ym
{
	class DX11Shader : public Shader
	{
	public:
		DX11Shader();
		virtual ~DX11Shader();

		void load(const std::string& fileName, ShaderLayout layout) override;
		void bind() override;

	private:
		void compileShader(ID3DBlob* errorMessageBlob, WCHAR* fileName);
	};
}