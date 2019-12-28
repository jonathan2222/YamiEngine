#pragma once

#include "../../Utils/Imgui/imgui.h"

namespace ym
{
	class ImGuiImpl
	{
	public:
		ImGuiImpl() = default;
		static ImGuiImpl* create();

		virtual void setUp() = 0;
		virtual void startFrame() = 0;
		virtual void endFrame() = 0;
		virtual void cleanUp() = 0;
	};
}