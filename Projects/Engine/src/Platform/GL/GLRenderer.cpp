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

ym::GLRenderer::GLRenderer()
{
}

void ym::GLRenderer::init(DisplayDesc& displayDescriptor)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glViewport(0, 0, (GLsizei)displayDescriptor.width, (GLsizei)displayDescriptor.height);
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

void ym::GLRenderer::draw(VertexArray* va, IndexBuffer* ib, Topology topology, Shader* shader)
{
	YM_LOG_ERROR("Missing implementation of 'draw' for the GLRenderer!");
}

void ym::GLRenderer::draw(Model* model, Shader* shader)
{
	YM_LOG_ERROR("Missing implementation of 'draw' for the GLRenderer!");
}
