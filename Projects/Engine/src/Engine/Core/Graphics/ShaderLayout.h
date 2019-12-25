#pragma once

#include <string>
#include <vector>
#include "Types.h"

namespace ym
{
	class ShaderLayout
	{
	public:
		struct Element
		{
			std::string semanticName;
			Format format;
		};

		std::vector<Element> elements;
	};
}