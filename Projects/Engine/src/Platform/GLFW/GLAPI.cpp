#include "GLAPI.h"

#include <GLFW/glfw3.h>

ym::API* ym::API::get()
{
	static GLAPI api;
	return &api;
}

void ym::GLAPI::init()
{
	glfwInit();
}

void ym::GLAPI::destroy()
{
	glfwTerminate();
}
