#pragma once

#include "../../Engine/Core/API.h"

namespace ym
{
	class GLAPI : public API
	{
	public:
		void init() override;
		void destroy() override;
	};
}
