#pragma once

namespace ym
{
	enum class Format
	{
		FLOAT_32_R,
		FLOAT_32_RG,
		FLOAT_32_RGB,
		FLOAT_32_RGBA,
		SINT_32_R,
		SINT_32_RG,
		SINT_32_RGB,
		SINT_32_RGBA,
		UINT_32_R,
		UINT_32_RG,
		UINT_32_RGB,
		UINT_32_RGBA,
		SINT_8_R,
		SINT_8_RG,
		SINT_8_RGBA,
		UINT_8_R,
		UINT_8_RG,
		UINT_8_RGBA
	};

	enum class Topology
	{
		POINT_LIST,
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP
	};

	enum class Usage
	{
		STATIC,
		DYNAMIC
	};

	enum class Type
	{
		FLOAT,
		INT,
		UINT,
		CHAR,
		UCHAR
	};

	unsigned int sizeOfType(Type type);
	unsigned int countOfFormat(Format format);
	Type typeOfFormat(Format format);
}