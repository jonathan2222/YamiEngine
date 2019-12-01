#include "GLAPI.h"

#include <GLFW/glfw3.h>

ym::GLAPI* ym::GLAPI::get()
{
	static GLAPI api;
	return &api;
}

void ym::GLAPI::init(DisplayDesc& displayDescriptor)
{
	glfwInit();
}

void ym::GLAPI::destroy()
{
	glfwTerminate();
}
