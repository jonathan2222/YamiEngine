#pragma once

#include "../../Engine/Core/API.h"
#include <GL/glew.h>

namespace ym
{
	struct DisplayDesc;
	class GLAPI : public API
	{
	public:
		static GLAPI* get();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;

		void initDebug();

	private:
		static void errorCallback(int error, const char* description);
		static void debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};
}
