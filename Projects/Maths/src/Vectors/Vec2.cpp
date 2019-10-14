#include "Vec2.h"

#include "Vec3.h"
#include "Vec4.h"

ym::Vec2 ym::Vec2::VEC2_X = Vec2(1.0f, 0.0f);
ym::Vec2 ym::Vec2::VEC2_Y = Vec2(0.0f, 1.0f);
ym::Vec2 ym::Vec2::VEC2_ONES = Vec2(1.0f, 1.0f);
ym::Vec2 ym::Vec2::VEC2_ZEROS = Vec2(0.0f, 0.0f);

const ym::Vec2::vec ym::Vec2::v = { &ym::Vec2::x, &ym::Vec2::y };

ym::Vec2::Vec2() : x(0.0f), y(0.0f)
{
}

ym::Vec2::Vec2(float value) : x(value), y(value)
{
}

ym::Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

ym::Vec2::Vec2(const Vec2 & v) : x(v.x), y(v.y)
{
}

ym::Vec2::Vec2(const Vec3 & v) : x(v.x), y(v.y)
{
}

ym::Vec2::Vec2(const Vec4 & v) : x(v.x), y(v.y)
{
}

float & ym::Vec2::operator[](size_t i)
{
	return this->*v[i];
}

const float & ym::Vec2::operator[](size_t i) const
{
	return this->*v[i];
}

bool ym::Vec2::operator==(const Vec2 & v)
{
	return this->x == v.x && this->y == v.y;
}

bool ym::Vec2::operator!=(const Vec2 & v)
{
	return !(*this == v);
}

ym::Vec2 & ym::Vec2::operator=(const Vec2 & v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}

ym::Vec2 & ym::Vec2::operator+=(const Vec2 & v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

ym::Vec2 & ym::Vec2::operator-=(const Vec2 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

ym::Vec2 & ym::Vec2::operator+=(float scalar)
{
	this->x += scalar;
	this->y += scalar;
	return *this;
}

ym::Vec2 & ym::Vec2::operator-=(float scalar)
{
	this->x -= scalar;
	this->y -= scalar;
	return *this;
}

ym::Vec2 & ym::Vec2::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	return *this;
}

ym::Vec2 & ym::Vec2::operator/=(float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	return *this;
}

ym::Vec2 ym::Vec2::operator+(const Vec2 & v) const
{
	return Vec2(this->x + v.x, this->y + v.y);
}

ym::Vec2 ym::Vec2::operator-(const Vec2 & v) const
{
	return Vec2(this->x - v.x, this->y - v.y);
}

ym::Vec2 ym::Vec2::operator+(float scalar) const
{
	return Vec2(this->x + scalar, this->y + scalar);
}

ym::Vec2 ym::Vec2::operator-(float scalar) const
{
	return Vec2(this->x - scalar, this->y - scalar);
}

ym::Vec2 ym::Vec2::operator*(float scalar) const
{
	return Vec2(this->x * scalar, this->y * scalar);
}

ym::Vec2 ym::Vec2::operator/(float scalar) const
{
	return Vec2(this->x / scalar, this->y / scalar);
}

ym::Vec2 & ym::Vec2::operator++()
{
	this->x++;
	this->y++;
	return *this;
}

ym::Vec2 & ym::Vec2::operator--()
{
	this->x--;
	this->y--;
	return *this;
}

ym::Vec2 ym::Vec2::operator++(int a) const
{
	return Vec2(this->x + a, this->y +a);
}

ym::Vec2 ym::Vec2::operator--(int a) const
{
	return Vec2(this->x - a, this->y - a);
}

ym::Vec2 ym::Vec2::operator-() const
{
	return Vec2(-this->x, -this->y);
}

int ym::Vec2::length() const
{
	return 2;
}

ym::Vec3 ym::Vec2::toVec3() const
{
	return Vec3(*this);
}

ym::Vec4 ym::Vec2::toVec4() const
{
	return Vec4(*this);
}
