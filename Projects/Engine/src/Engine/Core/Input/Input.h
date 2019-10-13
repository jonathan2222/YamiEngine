#pragma once

#include "Keys.h"

namespace ym
{
	class Input
	{
	public:
		static Input* get();

		virtual void init() = 0;
		virtual bool isKeyPressed(const Key& key) const = 0;
		virtual bool isKeyReleased(const Key& key) const = 0;
	};
}
