#include "GLRenderer.h"

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
