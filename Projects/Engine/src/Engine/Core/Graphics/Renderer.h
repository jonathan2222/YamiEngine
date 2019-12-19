#pragma once

namespace ym
{
	struct DisplayDesc;
	class Renderer
	{
	public:
		static Renderer* get();

		virtual void init(DisplayDesc& displayDescriptor) = 0;
		virtual void destroy() = 0;

		virtual void beginScene(float r, float g, float b, float a) = 0;
		virtual void endScene() = 0;
	};
}