#pragma once

#include <string>
#include "ShaderLayout.h"

namespace ym
{
	class Shader
	{
	public:
		static Shader* create();

		/*
			Will load the vertex shader and pixel shader.
			Arguments: 
				fileName: Should be the path to the file, should not include the file extension!
		*/
		virtual void load(const std::string& fileName, ShaderLayout layout) = 0;
		virtual void bind() = 0;
	};
}