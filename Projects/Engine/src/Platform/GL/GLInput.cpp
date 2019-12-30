#include "stdafx.h"
#include "GLInput.h"

#include <GLFW/glfw3.h>
#include "../../Engine/Core/Display.h"
#include "../../Engine/Core/Input/Keys.h"
#include "../../Engine/Core/ImGuiImpl.h"

std::unordered_map<ym::Key, ym::KeyState> ym::GLInput::m_keyMap = std::unordered_map<ym::Key, ym::KeyState>();
std::unordered_map<ym::MB, ym::KeyState> ym::GLInput::m_mbMap = std::unordered_map<ym::MB, ym::KeyState>();
glm::vec2 ym::GLInput::m_mousePos = glm::vec2(0.0f);

void ym::GLInput::init()
{
	YM_PROFILER_FUNCTION();

	GLFWwindow* wnd = static_cast<GLFWwindow*>(Display::get()->getNativeDisplay());
	glfwSetKeyCallback(wnd, GLInput::keyCallback);
	glfwSetCursorPosCallback(wnd, GLInput::cursorPositionCallback);
	glfwSetMouseButtonCallback(wnd, GLInput::mouseButtonCallback);

	for (int i = (int)Key::FIRST; i <= (int)Key::LAST; i++)
		m_keyMap[(Key)i] = KeyState::RELEASED;

	for (int i = (int)MB::LEFT; i <= (int)MB::MIDDLE; i++)
		m_mbMap[(MB)i] = KeyState::RELEASED;
}

bool ym::GLInput::isKeyPressed(const Key& key) const
{
	return m_keyMap[key] == KeyState::PRESSED;
}

bool ym::GLInput::isKeyReleased(const Key& key) const
{
	return m_keyMap[key] == KeyState::RELEASED;
}

glm::vec2 ym::GLInput::getMousePos() const
{
	return m_mousePos;
}

bool ym::GLInput::isMBPressed(const MB& button) const
{
	return m_mbMap[button] == KeyState::PRESSED;
}

bool ym::GLInput::isMBReleased(const MB& button) const
{
	return m_mbMap[button] == KeyState::RELEASED;
}

void ym::GLInput::lockMouse() const
{
	GLFWwindow* wnd = static_cast<GLFWwindow*>(Display::get()->getNativeDisplay());
	glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ym::GLInput::unlockMouse() const
{
	GLFWwindow* wnd = static_cast<GLFWwindow*>(Display::get()->getNativeDisplay());
	glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void ym::GLInput::keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
	ImGuiImpl* imGuiImpl = Display::get()->getImGuiImpl();
	if (imGuiImpl == nullptr || (imGuiImpl != nullptr && imGuiImpl->needInput() == false))
	{
		Key ymKey = (Key)key;
		if (action == GLFW_PRESS)
			m_keyMap[ymKey] = KeyState::PRESSED;
		if (action == GLFW_RELEASE)
			m_keyMap[ymKey] = KeyState::RELEASED;
	}
}

void ym::GLInput::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	ImGuiImpl* imGuiImpl = Display::get()->getImGuiImpl();
	if (imGuiImpl == nullptr || (imGuiImpl != nullptr && imGuiImpl->needInput() == false))
	{
		m_mousePos.x = (float)xpos;
		m_mousePos.y = (float)ypos;
	}
}

void ym::GLInput::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	ImGuiImpl* imGuiImpl = Display::get()->getImGuiImpl();
	if (imGuiImpl == nullptr || (imGuiImpl != nullptr && imGuiImpl->needInput() == false))
	{
		MB ymButton = (MB)button;
		if (action == GLFW_PRESS)
			m_mbMap[ymButton] = KeyState::PRESSED;
		if (action == GLFW_RELEASE)
			m_mbMap[ymButton] = KeyState::RELEASED;
	}
}
