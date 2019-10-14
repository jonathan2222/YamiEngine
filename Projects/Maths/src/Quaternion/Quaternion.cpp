#include "Quaternion.h"

#include "../Maths.h"

ym::Quaternion::Quaternion()
{
	init(0.0f, 0.0f, 0.0f, 0.0f);
}

ym::Quaternion::Quaternion(const Quaternion & q) : x(0.0f), y(0.0f), z(0.0f), s(0.0f)
{
	init(q.s, q.x, q.y, q.z);
}

ym::Quaternion::Quaternion(float s) : x(0.0f), y(0.0f), z(0.0f), s(0.0f)
{
	init(s, 0.0f, 0.0f, 0.0f);
}

ym::Quaternion::Quaternion(float pitch, float roll, float yaw) : x(0.0f), y(0.0f), z(0.0f), s(0.0f)
{
	setYawPitchRoll(pitch, roll, yaw);
}

ym::Quaternion::Quaternion(float s, float x, float y, float z) : x(0.0f), y(0.0f), z(0.0f), s(0.0f)
{
	init(s, x, y, z);
}

ym::Quaternion::Quaternion(const Vec3 & v) : x(0.0f), y(0.0f), z(0.0f), s(0.0f)
{
	init(0.0f, v.x, v.y, v.z);
}

ym::Quaternion::Quaternion(const Vec3 & from, const Vec3 & to) : x(0.0f), y(0.0f), z(0.0f), s(0.0f)
{
	setRotation(from, to);
}

ym::Quaternion::Quaternion(float angle, const Vec3 & axis) : x(0.0f), y(0.0f), z(0.0f), s(0.0f)
{
	init(cos(angle / 2.f), sin(angle / 2.f)*axis.x, sin(angle / 2.f)*axis.y, sin(angle / 2.f)*axis.z);
}

void ym::Quaternion::setAngleAxis(float angle, const Vec3 & axis)
{
	init(cos(angle/2.f), sin(angle/2.f)*axis.x, sin(angle / 2.f)*axis.y, sin(angle / 2.f)*axis.z);
}

void ym::Quaternion::setYawPitchRoll(float pitch, float roll, float yaw)
{
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	init((float)(cy * cr * cp + sy * sr * sp), 
		(float)(cy * sr * cp - sy * cr * sp),
		(float)(cy * cr * sp + sy * sr * cp),
		(float)(sy * cr * cp - cy * sr * sp));
}

void ym::Quaternion::setRotation(const Vec3 & from, const Vec3 & to)
{
	Quaternion q;
	float d = Maths::dot(from, to);
	if (d > 1.0f- MathsConstatns::EPSILON) // Same direction.
	{
		init(1.0f, 0.0f, 0.0f, 0.0f);
		return;
	}
	if (d < -1.0f + MathsConstatns::EPSILON) // 'to' is inverse from 'from' => rotate 180 deg.
	{
		Vec3 axis = Maths::cross(Vec3::VEC3_X, from);
		if (Maths::isZeroLength(axis))
			axis = Maths::cross(Vec3::VEC3_Y, from);
		axis = Maths::normalize<Vec3>(axis);
		q.setAngleAxis(MathsConstatns::PI, axis);
		init(q.s, q.x, q.y, q.z);
	}
	else
	{
		Vec3 v = Maths::cross(from, to);
		q.v = v;
		float lenF = Maths::length(from);
		float lenT = Maths::length(to);
		q.s = 1.0f + d;// sqrt(lenF*lenF * lenT*lenT) + Maths::dot(from, to);
		q.normalize();
		init(q.s, q.x, q.y, q.z);
	}
}

ym::Vec3 ym::Quaternion::rotate(const Vec3 & v) const
{
	return this->matrix*v;
}

ym::Vec3 ym::Quaternion::operator*(const Vec3 & v)
{
	return rotate(v);
}

ym::Vec3 ym::Quaternion::toVec3() const
{
	return rotate(Vec3(1.0f, 0.0f, 0.0f));
}

void ym::Quaternion::addAngle(float angle)
{
	float preAngle = getAngle();
	setAngleAxis(preAngle + angle, getAxis());
}

void ym::Quaternion::addRotation(float angle, const Vec3 & axis)
{
	Quaternion q;
	q.setAngleAxis(angle, axis);
	addRotation(q);
}

void ym::Quaternion::addRotation(float pitch, float roll, float yaw)
{
	Quaternion q;
	q.setYawPitchRoll(pitch, roll, yaw);
	addRotation(q);
}

void ym::Quaternion::addRotation(const Quaternion & q)
{
	if (this->s == 0.0f && this->x == 0.0f && this->y == 0.0f && this->z == 0.0f)
	{
		setAngleAxis(0.0f, Vec3(1.0f, 0.0f, 0.0f));
	}
	
	float a = s * q.s - x * q.x - y * q.y - z * q.z;
	float b = s * q.x + x * q.s + y * q.z - z * q.y;
	float c = s * q.y - x * q.z + y * q.s + z * q.x;
	float d = s * q.z + x * q.y - y * q.x + z * q.s;
	this->s = a;
	this->x = b;
	this->y = c;
	this->z = d;
	normalize();
	calculateMatrix();
}

ym::Quaternion ym::Quaternion::operator*(const Quaternion & q)
{
	float a = s * q.s - x * q.x - y * q.y - z * q.z;
	float b = s * q.x + x * q.s + y * q.z - z * q.y;
	float c = s * q.y - x * q.z + y * q.s + z * q.x;
	float d = s * q.z + x * q.y - y * q.x + z * q.s;
	return Quaternion(a, b, c, d);
}

ym::Quaternion & ym::Quaternion::operator*=(const Quaternion & q)
{
	addRotation(q);
	calculateMatrix();
	return *this;
}

ym::Mat3 ym::Quaternion::operator*(const Mat3 & m)
{
	return this->matrix * m;
}

ym::Mat4 ym::Quaternion::operator*(const Mat4 & m)
{
	return getMatrix4() * m;
}

ym::Quaternion ym::Quaternion::operator*(float a)
{
	return Quaternion(s*a, x*a, y*a, z*a);
}

ym::Quaternion ym::Quaternion::operator/(float a)
{
	return Quaternion(s/a, x/a, y/a, z/a);
}

ym::Quaternion & ym::Quaternion::operator*=(float a)
{
	this->s *= a;
	this->v *= a;
	calculateMatrix();
	return *this;
}

ym::Quaternion & ym::Quaternion::operator/=(float a)
{
	this->s /= a;
	this->v /= a;
	calculateMatrix();
	return *this;
}

ym::Vec3 ym::Quaternion::getAxis() const
{
	const Vec3 axis(this->x, this->y, this->z);
	const float len = Maths::length(axis);
	return axis / len;
}

float ym::Quaternion::getAngle() const
{
	const Vec3 axis(this->x, this->y, this->z);
	const float len = Maths::length(axis);
	return 2.0f * std::atan2f(len, this->s);
}

ym::Quaternion ym::Quaternion::getConjugate() const
{
	return Quaternion(s, -x, -y, -z);
}

ym::Quaternion ym::Quaternion::getInverse() const
{
	Quaternion inverse(getConjugate());
	return inverse/length2();
}

ym::Quaternion & ym::Quaternion::conjugate()
{
	this->v *= -1;
	calculateMatrix();
	return *this;
}

ym::Quaternion & ym::Quaternion::inverse()
{
	Quaternion conjugate(getConjugate());
	(*this) /= length2();
	calculateMatrix();
	return *this;
}

ym::Quaternion& ym::Quaternion::normalize()
{
	(*this) /= length();
	calculateMatrix();
	return *this;
}

float ym::Quaternion::length() const
{
	return sqrt(length2());
}

float ym::Quaternion::length2() const
{
	return s * s + x * x + y * y + z * z;
}

void ym::Quaternion::init(float s, float x, float y, float z)
{
	this->s = s;
	this->x = x;
	this->y = y;
	this->z = z;
	calculateMatrix();
}

void ym::Quaternion::calculateMatrix()
{
	const float len = length();
	const float a = 1.0f / (len*len);
	this->matrix[0][0] = 1 - 2*a*(y*y + z*z);	this->matrix[1][0] = 2*a*(x*y - z*s);		this->matrix[2][0] = 2*a*(x*z + y*s);
	this->matrix[0][1] = 2*a*(x*y + z*s);		this->matrix[1][1] = 1 - 2*a*(x*x + z*z);	this->matrix[2][1] = 2*a*(y*z - x*s);
	this->matrix[0][2] = 2*a*(x*z - y*s);		this->matrix[1][2] = 2*a*(y*z + x*s);		this->matrix[2][2] = 1 - 2*a*(x*x + y*y);
}

ym::Mat3 ym::Quaternion::getMatrix3() const
{
	return this->matrix;
}

ym::Mat4 ym::Quaternion::getMatrix4() const
{
	return Mat4(this->matrix);
}
