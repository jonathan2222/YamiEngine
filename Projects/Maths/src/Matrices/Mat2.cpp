#include "Mat2.h"

#include "Mat3.h"
#include "Mat4.h"

ym::Mat2::Mat2()
{
	this->col[0].x = 1.0f;	this->col[1].x = 0.0f;
	this->col[0].y = 0.0f;	this->col[1].y = 1.0f;
}

ym::Mat2::Mat2(float value)
{
	this->col[0].x = value;	this->col[1].x = 0.0f;
	this->col[0].y = 0.0f;	this->col[1].y = value;
}

ym::Mat2::Mat2(const Vec2 & col1, const Vec2 & col2)
{
	this->col[0] = col1;
	this->col[1] = col2;
}

ym::Mat2::Mat2(float e11, float e12, float e21, float e22)
{
	this->col[0].x = e11;	this->col[1].x = e12;
	this->col[0].y = e21;	this->col[1].y = e22;
}

ym::Mat2::Mat2(const Mat4 & m)
{
	this->col[0].x = m[0].x;	this->col[1].x = m[1].x;
	this->col[0].y = m[0].y;	this->col[1].y = m[1].y;
}

ym::Mat2::Mat2(const Mat3 & m)
{
	this->col[0].x = m[0].x;	this->col[1].x = m[1].x;
	this->col[0].y = m[0].y;	this->col[1].y = m[1].y;
}

ym::Mat2::Mat2(const Mat2 & m)
{
	copy(m);
}

ym::Vec2 & ym::Mat2::operator[](size_t i)
{
	return getColumn(i);
}

const ym::Vec2 & ym::Mat2::operator[](size_t i) const
{
	return getColumn(i);
}

ym::Vec2 ym::Mat2::getRow(size_t i) const
{
	return Vec2(this->col[0][i], this->col[1][i]);
}

ym::Vec2 & ym::Mat2::getColumn(size_t i)
{
	return this->col[i];
}

const ym::Vec2 & ym::Mat2::getColumn(size_t i) const
{
	return this->col[i];
}

ym::Mat2 & ym::Mat2::operator=(const Mat2 & m)
{
	return copy(m);
}

ym::Mat2 & ym::Mat2::operator*=(const Mat2 & m)
{
	return copy(*this * m);
}

ym::Mat2 & ym::Mat2::operator*=(float scalar)
{
	for (size_t c = 0; c < 2; c++)
		for (size_t r = 0; r < 2; r++)
			this->col[c][r] *= scalar;
	return *this;
}

ym::Mat2 ym::Mat2::operator*(const Mat2 & m) const
{
	Mat2 ret;
	for (size_t r = 0; r < 2; r++)
		for (size_t c = 0; c < 2; c++)
			ret[c][r] = this->col[0][r] * m[c][0] + this->col[1][r] * m[c][1];
	return ret;
}

ym::Vec2 ym::Mat2::operator*(const Vec2 & v) const
{
	Vec2 ret;
	for (size_t r = 0; r < 2; r++)
		ret[r] = this->col[0][r] * v[0] + this->col[1][r] * v[1];
	return ret;
}

ym::Mat2 ym::Mat2::operator*(float scalar) const
{
	Mat2 m(*this);
	return m *= scalar;
}

int ym::Mat2::getSideLength() const
{
	return 2;
}

ym::Mat2& ym::Mat2::copy(const Mat2 & m)
{
	for (size_t c = 0; c < 2; c++)
		this->col[c] = m[c];
	return *this;
}
