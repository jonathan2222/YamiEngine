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

		Vec2 getMousePos() const;
		bool isMBPressed(const MB& button) const;

		void lockMouse() const;
		void unlockMouse() const;

	private:
		static std::unordered_map<Key, KeyState> m_keyMap;
		static std::unordered_map<MB, KeyState> m_mbMap;
		static Vec2 m_mousePos;
	};
}