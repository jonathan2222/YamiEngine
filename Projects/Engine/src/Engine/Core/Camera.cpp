#include "Camera.h"

ym::Camera::Camera(const Vec3& pos, const Vec3& dir, float nearPlane, float farPlane) : 
	m_pos(pos), m_dir(dir), m_nearPlane(nearPlane), m_farPlane(farPlane)
{
}

float ym::Camera::getNearPlane() const
{
	return m_nearPlane;
}

float ym::Camera::getFarPlane() const
{
	return m_farPlane;
}

ym::Vec3 ym::Camera::getPos() const
{
	return m_pos;
}

ym::Vec3 ym::Camera::getDir() const
{
	return m_dir;
}
