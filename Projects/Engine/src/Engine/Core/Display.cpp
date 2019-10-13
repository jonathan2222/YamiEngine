#include "Display.h"

ym::Display* ym::Display::m_self = nullptr;

ym::Display* ym::Display::get()
{
	return m_self;
}