#pragma once

#include"../../Engine/Core/Graphics/Renderer.h"

namespace ym
{
	class GLRenderer : public Renderer
	{
	public:
		static GLRenderer* get();

		void init(DisplayDesc& displayDescriptor) override;
		void destroy() override;
	};
}