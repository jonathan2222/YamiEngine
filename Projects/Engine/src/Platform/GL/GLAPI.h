#pragma once

#include "../../Engine/Core/API.h"

namespace ym
{
	class GLAPI : public API
	{
	public:
		static GLAPI* get();

		void init() override;
		void destroy() override;
	};
}
