#pragma once

#include "Vectors/Vec3.h"

namespace ym
{
	class Camera
	{
	public:
		Camera(const Vec3& pos, const Vec3& dir, float nearPlane, float farPlane);

		float getNearPlane() const;
		float getFarPlane() const;

		Vec3 getPos() const;
		Vec3 getDir() const;

	private:
		Vec3 m_pos;
		Vec3 m_dir;
		float m_nearPlane;
		float m_farPlane;
	};
}