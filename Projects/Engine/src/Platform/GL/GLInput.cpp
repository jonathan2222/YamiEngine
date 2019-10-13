#include "GLInput.h"

#include <GLFW/glfw3.h>
#include "../../Engine/Core/Display.h"
#include "../../Engine/Core/Input/Keys.h"

std::unordered_map<ym::Key, bool> ym::GLInput::m_keyMap = std::unordered_map<ym::Key, bool>();

ym::Input* ym::Input::get()
{
	static ym::GLInput input;
	return &input;
}

void ym::GLInput::init()
{
	GLFWwindow* wnd = static_cast<GLFWwindow*>(Display::get()->getNativeDisplay());
	glfwSetKeyCallback(wnd, GLInput::keyCallback);

	for (int i = Key::FIRST; i <= Key::LAST; i++)
		m_keyMap[(Key)i] = KeyState::RELEASED;
}

bool ym::GLInput::isKeyPressed(const Key& key) const
{
	return m_keyMap[key] == KeyState::PRESSED;
}

bool ym::GLInput::isKeyReleased(const Key& key) const
{
	return m_keyMap[key] == KeyState::RELEASED;
}

void ym::GLInput::keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
	Key ymKey = (Key)key;
	if (action == GLFW_PRESS)
		m_keyMap[ymKey] = KeyState::PRESSED;
	if (action == GLFW_RELEASE)
		m_keyMap[ymKey] = KeyState::RELEASED;
}