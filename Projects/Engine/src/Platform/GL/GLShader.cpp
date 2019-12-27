#include "stdafx.h"
#include "GLShader.h"

ym::GLShader::GLShader()
{
	YM_LOG_ERROR("Missing implementation of 'Shader' in GLShader!");
}

ym::GLShader::~GLShader()
{
}

void ym::GLShader::load(const std::string& fileName, AttributeLayout& layout)
{
	YM_LOG_ERROR("Missing implementation of 'load' in GLShader!");
}

void ym::GLShader::bind()
{
	YM_LOG_ERROR("Missing implementation of 'bind' in GLShader!");
}

void* ym::GLShader::getId()
{
	// This should return a pointer to the shader id.
	YM_LOG_ERROR("Missing implementation of 'getId' in GLShader!");
	return nullptr;
}
