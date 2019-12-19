#pragma once

#include "glm/vec3.hpp"

namespace ym
{
	class Camera
	{
	public:
		Camera(const glm::vec3& pos, const glm::vec3& dir, float nearPlane, float farPlane);

		float getNearPlane() const;
		float getFarPlane() const;

		glm::vec3 getPos() const;
		glm::vec3 getDir() const;

	private:
		glm::vec3 m_pos;
		glm::vec3 m_dir;
		float m_nearPlane;
		float m_farPlane;
	};
}