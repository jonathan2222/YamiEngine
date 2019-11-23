#pragma once

#include "../../Engine/Core/API.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ym
{
	class DX11API : public API
	{
		void init() override;
		void destroy() override;
	};
}