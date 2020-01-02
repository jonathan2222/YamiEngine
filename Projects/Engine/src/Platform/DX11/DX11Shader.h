#pragma once

#include "../../Engine/Core/Graphics/Shader.h"
#include "DX11API.h"

#include <unordered_map>
#include <optional>
#include <vector>

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

		void setTexture(const std::string& name, Texture* texture, Sampler sampler, unsigned int unit) override;

	private:
		struct UniformData
		{
			unsigned int index;
			bool justAdded; // This is true if it was just added. Need to be set to false by the programmer!
			D3D11_SHADER_INPUT_BIND_DESC binding;
			unsigned int shaderType;
		};

		UniformData* addUniform(const std::string& name);

		void createPixelShader(const std::string& path);
		void createVertexShader(const std::string& path);
		void compileShader(ID3DBlob* errorMessageBlob);

		void createLayout(AttributeLayout& layout);

		std::wstring s2ws(const std::string& s) const;

		ID3D11VertexShader* m_vsShader;
		ID3D11PixelShader* m_psShader;
		ID3D11InputLayout* m_layout;
		ID3DBlob* m_vertexShaderBuffer;
		ID3D11ShaderReflection* m_psReflector;
		ID3D11ShaderReflection* m_vsReflector;

		std::unordered_map<std::string, UniformData> m_uniforms;
		std::vector<ID3D11ShaderResourceView*> m_srvs;
		std::vector<ID3D11SamplerState*> m_samplers;
	};
}