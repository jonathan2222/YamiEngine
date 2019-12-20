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
