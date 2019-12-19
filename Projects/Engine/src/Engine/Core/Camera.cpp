#include "Camera.h"

ym::Camera::Camera(const glm::vec3& pos, const glm::vec3& dir, float nearPlane, float farPlane) :
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

glm::vec3 ym::Camera::getPos() const
{
	return m_pos;
}

glm::vec3 ym::Camera::getDir() const
{
	return m_dir;
}
