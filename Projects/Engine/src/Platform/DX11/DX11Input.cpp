#include "DX11Input.h"

#include "DX11API.h"

std::unordered_map<ym::Key, ym::KeyState> ym::DX11Input::m_keyMap = std::unordered_map<ym::Key, ym::KeyState>();
std::unordered_map<ym::MB, ym::KeyState> ym::DX11Input::m_mbMap = std::unordered_map<ym::MB, ym::KeyState>();
glm::vec2 ym::DX11Input::m_mousePos = glm::vec2();

void ym::DX11Input::init()
{
	for (int i = (int)Key::FIRST; i <= (int)Key::LAST; i++)
		m_keyMap[(Key)i] = KeyState::RELEASED;

	for (int i = (int)MB::LEFT; i <= (int)MB::MIDDLE; i++)
		m_mbMap[(MB)i] = KeyState::RELEASED;
}

bool ym::DX11Input::isKeyPressed(const Key& key) const
{
	return m_keyMap[key] == KeyState::PRESSED;
}

bool ym::DX11Input::isKeyReleased(const Key& key) const
{
	return m_keyMap[key] == KeyState::RELEASED;
}

glm::vec2 ym::DX11Input::getMousePos() const
{
	return m_mousePos;
}

bool ym::DX11Input::isMBPressed(const MB& button) const
{
	return m_mbMap[button] == KeyState::PRESSED;
}

bool ym::DX11Input::isMBReleased(const MB& button) const
{
	return m_mbMap[button] == KeyState::RELEASED;
}

void ym::DX11Input::lockMouse() const
{
	// TODO: Do this!
}

void ym::DX11Input::unlockMouse() const
{
	// TODO: Do this!	
}

void ym::DX11Input::setKey(int keyCode, KeyState state)
{
	Key key = (Key)keyCode;
	setKey(key, state);
}

void ym::DX11Input::setKey(Key key, KeyState state)
{
	m_keyMap[key] = state;
}

void ym::DX11Input::setMb(int mbCode, KeyState state)
{
	MB mb = (MB)mbCode;
	setMb(mb, state);
}

void ym::DX11Input::setMb(MB mb, KeyState state)
{
	m_mbMap[mb] = state;
}

void ym::DX11Input::setMousePos(int x, int y)
{
	m_mousePos.x = (float)x;
	m_mousePos.y = (float)y;
}

ym::KeyState ym::DX11Input::getKeyState(Key key) const
{
	return m_keyMap[key];
}
