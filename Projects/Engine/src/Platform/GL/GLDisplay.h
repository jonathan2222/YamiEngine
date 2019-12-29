#pragma once

#include "../../Engine/Core/Display.h"

namespace ym
{
	class GLDisplay : public Display
	{
	public:
		GLDisplay(const DisplayDesc& description);
		virtual ~GLDisplay();

		bool shouldClose() const noexcept override;
		void close() noexcept override;

		void pollEvents() noexcept override;

		void* getNativeDisplay() override;

	private:
		void init(const DisplayDesc& description);
		static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

		static GLDisplay* g_glDisplayPtr;

		bool m_shouldClose;
		GLFWwindow* m_window;
		DisplayDesc m_description;
	};
}