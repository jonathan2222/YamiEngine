#pragma once

#include "../../Engine/Core/ImGuiImpl.h"

namespace ym
{
	class GLImGuiImpl : public ImGuiImpl
	{
	public:
		GLImGuiImpl();
		virtual ~GLImGuiImpl();

		void setUp() override;
		void startFrame() override;
		void endFrame() override;
		void cleanUp() override;
	};
}