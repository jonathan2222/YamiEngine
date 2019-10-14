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

		Vec2 getMousePos() const;
		bool isMBPressed(const MB& button) const;

		void lockMouse() const;
		void unlockMouse() const;

	private:
		static void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
		static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	private:
		static std::unordered_map<Key, KeyState> m_keyMap;
		static std::unordered_map<MB, KeyState> m_mbMap;
		static Vec2 m_mousePos;
	};
}