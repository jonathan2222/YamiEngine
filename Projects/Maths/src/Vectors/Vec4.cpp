#include "Vec4.h"
#include "Vec3.h"
#include "Vec2.h"

ym::Vec4 ym::Vec4::VEC4_X = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
ym::Vec4 ym::Vec4::VEC4_Y = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
ym::Vec4 ym::Vec4::VEC4_Z = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
ym::Vec4 ym::Vec4::VEC4_W = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
ym::Vec4 ym::Vec4::VEC4_ONES = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
ym::Vec4 ym::Vec4::VEC4_ZEROS = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

const ym::Vec4::vec ym::Vec4::v = { &ym::Vec4::x, &ym::Vec4::y, &ym::Vec4::z, &ym::Vec4::w };

ym::Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

ym::Vec4::Vec4(float value) : x(value), y(value), z(value), w(value)
{
}

ym::Vec4::Vec4(float x, float y) : x(x), y(y), z(0.0f), w(0.0f)
{
}

ym::Vec4::Vec4(float x, float y, float z) : x(x), y(y), z(z), w(0.0f)
{
}

ym::Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

ym::Vec4::Vec4(const Vec4 & v) : x(v.x), y(v.y), z(v.z), w(v.w)
{
}

ym::Vec4::Vec4(const Vec3 & v, float w) : x(v.x), y(v.y), z(v.z), w(w)
{
}

ym::Vec4::Vec4(const Vec2 & v, float z, float w) : x(v.x), y(v.y), z(z), w(w)
{
}

float & ym::Vec4::operator[](size_t i)
{
	return this->*v[i];
}

const float & ym::Vec4::operator[](size_t i) const
{
	return this->*v[i];
}

bool ym::Vec4::operator==(const Vec4 & v)
{
	return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
}

bool ym::Vec4::operator!=(const Vec4 & v)
{
	return !(*this == v);
}

ym::Vec4 & ym::Vec4::operator=(const Vec4 & v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
	return *this;
}

ym::Vec4 & ym::Vec4::operator+=(const Vec4 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

ym::Vec4 & ym::Vec4::operator-=(const Vec4 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

ym::Vec4 & ym::Vec4::operator+=(float scalar)
{
	this->x += scalar;
	this->y += scalar;
	this->z += scalar;
	this->w += scalar;
	return *this;
}

ym::Vec4 & ym::Vec4::operator-=(float scalar)
{
	this->x -= scalar;
	this->y -= scalar;
	this->z -= scalar;
	this->w -= scalar;
	return *this;
}

ym::Vec4 & ym::Vec4::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	this->w *= scalar;
	return *this;
}

ym::Vec4 & ym::Vec4::operator/=(float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
	this->w /= scalar;
	return *this;
}

ym::Vec4 ym::Vec4::operator+(const Vec4 & v) const
{
	return Vec4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
}

ym::Vec4 ym::Vec4::operator-(const Vec4 & v) const
{
	return Vec4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
}

ym::Vec4 ym::Vec4::operator+(float scalar) const
{
	return Vec4(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar);
}

ym::Vec4 ym::Vec4::operator-(float scalar) const
{
	return Vec4(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar);
}

ym::Vec4 ym::Vec4::operator*(float scalar) const
{
	return Vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

ym::Vec4 ym::Vec4::operator/(float scalar) const
{
	return Vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
}

ym::Vec4 & ym::Vec4::operator++()
{
	this->x++;
	this->y++;
	this->z++;
	this->w++;
	return *this;
}

ym::Vec4 & ym::Vec4::operator--()
{
	this->x--;
	this->y--;
	this->z--;
	this->w--;
	return *this;
}

ym::Vec4 ym::Vec4::operator++(int a) const
{
	return Vec4(this->x + a, this->y + a, this->z + a, this->w + a);
}

ym::Vec4 ym::Vec4::operator--(int a) const
{
	return Vec4(this->x - a, this->y - a, this->z - a, this->w - a);
}

ym::Vec4 ym::Vec4::operator-() const
{
	return Vec4(-this->x, -this->y, -this->z, -this->w);
}

int ym::Vec4::length() const
{
	return 4;
}

ym::Vec2 ym::Vec4::toVec2() const
{
	return Vec2(*this);
}

ym::Vec3 ym::Vec4::toVec3() const
{
	return Vec3(*this);
}
