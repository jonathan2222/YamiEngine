#pragma once

#include "Keys.h"
#include "Vectors/Vec2.h"

namespace ym
{
	class Input
	{
	public:
		static Input* get();

		virtual void init() = 0;
		virtual bool isKeyPressed(const Key& key) const = 0;
		virtual bool isKeyReleased(const Key& key) const = 0;

		virtual Vec2 getMousePos() const = 0;
		virtual bool isMBPressed(const MB& button) const = 0;

		virtual void lockMouse() const = 0;
		virtual void unlockMouse() const = 0;
	};
}
