#pragma once

#include "../../Engine/Core/API.h"
#include "../../Engine/Core/Graphics/Types.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ym
{
	struct DisplayDesc;
	class GLAPI : public API
	{
	public:
		static GLAPI* get();

		void preDisplayInit(DisplayDesc& displayDescriptor) override;
		void postDisplayInit() override;
		void destroy() override;

		void initDebug();

		GLenum convertType(Type type) const;

	private:
		static void errorCallback(int error, const char* description);
		static void debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};
}
