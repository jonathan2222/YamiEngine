#pragma once

#include "../../Engine/Core/Graphics/Shader.h"
#include "GLAPI.h"
#include <unordered_map>

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

		void setTexture(const std::string& name, Texture* texture, Sampler sampler, unsigned int unit) override;

	private:
		void init(const std::string& vertex, const std::string& fragment);
		GLint addUniform(const std::string& name);

		bool addShaderPart(const std::string& name, GLuint type);
		GLuint loadShaderPart(const std::string& name, GLuint type);
		bool compileShaderPart(GLuint id, GLuint type);
		bool link();

		GLuint m_id;
		std::vector<GLuint> m_ids;
		std::unordered_map<std::string, GLint> m_uniforms;
	};
}