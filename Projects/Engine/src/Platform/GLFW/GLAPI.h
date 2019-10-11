#pragma once

#include "../../Engine/Core/API.h"

namespace ym
{
	class GLAPI : public API
	{
		void init() override;
		void destroy() override;
	};
}
