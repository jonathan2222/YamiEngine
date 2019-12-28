#pragma once

#include "../../Engine/Core/ImGuiImpl.h"

namespace ym
{
	class DX11ImGuiImpl : public ImGuiImpl
	{
	public:
		DX11ImGuiImpl();
		virtual ~DX11ImGuiImpl();

		void setUp() override;
		void startFrame() override;
		void endFrame() override;
		void cleanUp() override;
	};
}