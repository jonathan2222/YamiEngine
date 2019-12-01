#pragma once

#include "../../Engine/Core/API.h"

namespace ym
{
	struct DisplayDesc;
	class GLAPI : public API
	{
	public:
		static GLAPI* get();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;
	};
}
