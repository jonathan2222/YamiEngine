#include "stdafx.h"
#include "GLRenderer.h"

#include "../../Engine/Core/Display.h"

ym::GLRenderer* ym::GLRenderer::get()
{
	static GLRenderer renderer;
	return &renderer;
}

ym::GLRenderer::GLRenderer()
{
}

void ym::GLRenderer::resize(unsigned int width, unsigned int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void ym::GLRenderer::init(DisplayDesc& displayDescriptor)
{
	YM_PROFILER_FUNCTION();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	glViewport(0, 0, (GLsizei)displayDescriptor.width, (GLsizei)displayDescriptor.height);

	activate();
}

void ym::GLRenderer::destroy()
{
	YM_PROFILER_FUNCTION();

	deactivate();
}

void ym::GLRenderer::beginScene(float r, float g, float b, float a)
{
	YM_PROFILER_RENDERING_FUNCTION();

	glClearColor(r, g, b, a);
	// Clear back buffer and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ym::GLRenderer::endScene()
{
	YM_PROFILER_RENDERING_FUNCTION();

	GLFWwindow* window = (GLFWwindow*)Display::get()->getNativeDisplay();
	glfwSwapBuffers(window);
}

void ym::GLRenderer::draw(VertexArray* va, IndexBuffer* ib, Topology topology, Shader* shader)
{
	YM_PROFILER_RENDERING_FUNCTION();

	va->bind();
	ib->bind();
	shader->bind();
	glDrawElements(getGLTopology(topology), ib->getCount(), GL_UNSIGNED_INT, 0);
}

void ym::GLRenderer::draw(Model* model, Shader* shader)
{
	YM_PROFILER_RENDERING_FUNCTION();

	model->bind();
	shader->bind();
	glDrawElements(getGLTopology(model->getInfo().topology), model->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, (void*)0);
}

GLenum ym::GLRenderer::getGLTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::POINT_LIST: return GL_POINTS; break;
	case Topology::LINE_LIST: return GL_LINES; break;
	case Topology::LINE_STRIP: return GL_LINE_STRIP; break;
	case Topology::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP; break;
	case Topology::TRIANGLE_LIST:
	default:
		return GL_TRIANGLES;
		break;
	}
}
