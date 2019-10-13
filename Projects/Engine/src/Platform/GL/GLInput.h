#pragma once

#include "../../Engine/Core/Input/Input.h"
#include <unordered_map>

struct GLFWwindow;
namespace ym
{
	class GLInput : public Input
	{
	public:
		void init() override;
		bool isKeyPressed(const Key& key) const override;
		bool isKeyReleased(const Key& key) const override;

	private:
		static void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

	private:
		static std::unordered_map<Key, bool> m_keyMap;
	};
}