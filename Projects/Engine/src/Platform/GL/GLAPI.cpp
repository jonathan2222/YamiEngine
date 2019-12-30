#include "stdafx.h"
#include "GLAPI.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

ym::GLAPI* ym::GLAPI::get()
{
	static GLAPI api;
	return &api;
}

void ym::GLAPI::preDisplayInit(DisplayDesc& displayDescriptor)
{
	YM_PROFILER_FUNCTION();
	glfwInit();
	glfwSetErrorCallback(GLAPI::errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Modern opengl
}

void ym::GLAPI::postDisplayInit()
{
	YM_PROFILER_FUNCTION();
}

void ym::GLAPI::destroy()
{
	glfwTerminate();
}

void ym::GLAPI::initDebug()
{
	YM_PROFILER_FUNCTION();
	const GLubyte* renderer = glGetString(GL_RENDERER);
	API::VideoCardInfo& videoCardInfo = GLAPI::get()->getVideoCardInfo();
	videoCardInfo.name = std::string((char*)renderer);
	const GLubyte* version = glGetString(GL_VERSION);
	YM_LOG_INFO("OpenGL version: {0}", version);

#ifdef YAMI_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLAPI::debugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
}

GLenum ym::GLAPI::convertType(Type type) const
{
	switch (type)
	{
	case Type::INT:		return GL_INT; break;
	case Type::CHAR:	return GL_BYTE; break;
	case Type::UINT:	return GL_UNSIGNED_INT; break;
	case Type::UCHAR:	return GL_UNSIGNED_BYTE; break;
	case Type::FLOAT:
	default:
		return GL_FLOAT;
		break;
	}
}

void ym::GLAPI::errorCallback(int error, const char* description)
{
	YM_ASSERT(false, "{0}", description);
}

void ym::GLAPI::debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	YM_LOG_ERROR("------------------------------ GLError ------------------------------");
	YM_LOG_ERROR("Debug message ({0}): {1}", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             YM_LOG_ERROR("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   YM_LOG_ERROR("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: YM_LOG_ERROR("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     YM_LOG_ERROR("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     YM_LOG_ERROR("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           YM_LOG_ERROR("Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               YM_LOG_ERROR("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: YM_LOG_ERROR("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  YM_LOG_ERROR("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         YM_LOG_ERROR("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         YM_LOG_ERROR("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              YM_LOG_ERROR("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          YM_LOG_ERROR("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           YM_LOG_ERROR("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               YM_LOG_ERROR("Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         YM_LOG_ERROR("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       YM_LOG_ERROR("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          YM_LOG_ERROR("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: YM_LOG_ERROR("Severity: notification"); break;
	}
}
