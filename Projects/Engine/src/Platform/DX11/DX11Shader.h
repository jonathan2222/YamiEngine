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

		void load(const std::string& fileName, AttributeLayout& layout) override;
		void bind() override;
		void* getId() override;

	private:
		void createPixelShader(const std::string& path);
		void createVertexShader(const std::string& path);
		void compileShader(ID3DBlob* errorMessageBlob);

		void createLayout(AttributeLayout& layout);

		std::wstring s2ws(const std::string& s) const;

		ID3D11VertexShader* m_vsShader;
		ID3D11PixelShader* m_psShader;
		ID3D11InputLayout* m_layout;
		ID3DBlob* m_vertexShaderBuffer;
	};
}