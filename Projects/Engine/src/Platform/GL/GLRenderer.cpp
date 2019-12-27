#include "stdafx.h"
#include "GLRenderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../Engine/Core/Display.h"

ym::GLRenderer* ym::GLRenderer::get()
{
	static GLRenderer renderer;
	return &renderer;
}

void ym::GLRenderer::init(DisplayDesc& displayDescriptor)
{
}

void ym::GLRenderer::destroy()
{
}

void ym::GLRenderer::beginScene(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	// Clear back buffer and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ym::GLRenderer::endScene()
{
	GLFWwindow* window = (GLFWwindow*)Display::get()->getNativeDisplay();
	glfwSwapBuffers(window);
}

void ym::GLRenderer::initShader(WCHAR* vertexShader, WCHAR* pixelShader)
{
	YM_LOG_ERROR("Missing implementation of 'initShader' for the GLRenderer!");
}

void ym::GLRenderer::bindShader(glm::mat4& world, glm::mat4& view, glm::mat4& proj)
{
	YM_LOG_ERROR("Missing implementation of 'bindShader' for the GLRenderer!");
}

void ym::GLRenderer::draw(VertexArray* va, IndexBuffer* ib, Topology topology, Shader* shader)
{
	YM_LOG_ERROR("Missing implementation of 'draw' for the GLRenderer!");
}

void ym::GLRenderer::draw(Model* model, Shader* shader)
{
	YM_LOG_ERROR("Missing implementation of 'draw' for the GLRenderer!");
}
