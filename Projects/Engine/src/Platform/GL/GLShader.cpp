#include "stdafx.h"
#include "GLShader.h"

#include <fstream>

ym::GLShader::GLShader() : m_id(0)
{
}

ym::GLShader::~GLShader()
{
	glDeleteProgram(m_id);
}

void ym::GLShader::load(const std::string& fileName, AttributeLayout& layout)
{
	// Fetch the right shader.
	std::string finalPath = std::string(YM_SHADER_PATH) + std::string("GL/") + fileName;
	std::string vertexShaderPath = finalPath + ".vs";
	std::string fragmentShaderPath = finalPath + ".fs";

	init(vertexShaderPath, fragmentShaderPath);
}

void ym::GLShader::bind()
{
	glUseProgram(m_id);
}

void* ym::GLShader::getId()
{
	return &m_id;
}

void ym::GLShader::init(const std::string& vertex, const std::string& fragment)
{
	if (m_id != 0)
	{
		glDeleteProgram(m_id);
		m_id = 0;
	}
	m_id = glCreateProgram();

	if (vertex.empty() || fragment.empty())
	{
		YM_ASSERT(false, "Path to the vertex or fragment shader was not given!");
		return;
	}

	if (addShaderPart(vertex, GL_VERTEX_SHADER) == false)
		return;
	if (addShaderPart(fragment, GL_FRAGMENT_SHADER) == false)
		return;

	link();
}

bool ym::GLShader::addShaderPart(const std::string& name, GLuint type)
{
	GLuint shaderPartID = loadShaderPart(name, type);
	if (shaderPartID != 0) {
		if (compileShaderPart(shaderPartID, type)) {
			glAttachShader(m_id, shaderPartID);
			glDeleteShader(shaderPartID);
			return true;
		}
	}
	return false;
}

GLuint ym::GLShader::loadShaderPart(const std::string& name, GLuint type)
{
	GLuint id = glCreateShader(type);
	if (id == 0) {
		YM_ASSERT(false, "Could not create shader: {0}", name.c_str());
		return 0;
	}

	std::ifstream file(name);
	std::string shaderText;
	if (file.is_open())
	{
		shaderText = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	else
	{
		YM_ASSERT(false, "Could not create shader: {0}", name.c_str());
		return 0;
	}
	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(id, 1, &shaderTextPtr, 0);
	m_ids.push_back(id);
	return id;
}

bool ym::GLShader::compileShaderPart(GLuint id, GLuint type)
{
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = GL_FALSE;
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		glGetShaderInfoLog(id, 1024, nullptr, buff);
		std::string shaderType = type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader";
		YM_ASSERT(false, "Could not compile {0}: {1}", shaderType.c_str(), buff);
		return false;
	}
	return true;
}

bool ym::GLShader::link()
{
	glLinkProgram(m_id);

	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = GL_FALSE;
	glGetProgramiv(m_id, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		memset(buff, 0, 1024);
		glGetProgramInfoLog(m_id, 1024, nullptr, buff);
		buff[1023] = '\n';
		YM_ASSERT(false, "Failed to link shader program: %s", buff);
		return false;
	}

	for (GLuint partID : m_ids)
		glDetachShader(m_id, partID);
	return true;
}
