#include "stdafx.h"
#include "Types.h"

unsigned int ym::sizeofType(Type type)
{
	switch (type)
	{
	case Type::INT: return 4; break;
	case Type::CHAR: return 1; break;
	case Type::UINT: return 4; break;
	case Type::UCHAR: return 1; break;
	case Type::FLOAT:
	default: return 4;
	}
}
