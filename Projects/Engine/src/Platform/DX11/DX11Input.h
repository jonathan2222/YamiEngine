#pragma once

#include "../../Engine/Core/Input/Input.h"
#include <unordered_map>

namespace ym
{
	class DX11Input : public Input
	{
	public:
		void init() override;
		bool isKeyPressed(const Key& key) const override;
		bool isKeyReleased(const Key& key) const override;

		Vec2 getMousePos() const override;
		bool isMBPressed(const MB& button) const override;
		bool isMBReleased(const MB& button) const override;

		void lockMouse() const override;
		void unlockMouse() const override;

		void setKey(int keyCode, KeyState state);
		void setKey(Key key, KeyState state);
		void setMb(int mbCode, KeyState state);
		void setMb(MB mb, KeyState state);
		void setMousePos(int x, int y);
		KeyState getKeyState(Key key) const;
	private:
		static std::unordered_map<Key, KeyState> m_keyMap;
		static std::unordered_map<MB, KeyState> m_mbMap;
		static Vec2 m_mousePos;
	};
}