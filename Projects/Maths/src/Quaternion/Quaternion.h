#ifndef QUATERNION_H
#define QUATERNION_H

#include "../Vectors/Vec3.h"
#include "../Vectors/Vec4.h"
#include "../Matrices/Mat3.h"

namespace ym
{
	class Quaternion
	{
	public:
		struct
		{
			float s;
			union
			{
				struct
				{
					float x, y, z;
				};
				struct
				{
					Vec3 v;
				};
			};
		};

		Quaternion();
		Quaternion(const Quaternion& q);
		Quaternion(float s);
		Quaternion(float pitch, float roll, float yaw);
		Quaternion(float s, float x, float y, float z);
		Quaternion(const Vec3& v);
		Quaternion(const Vec3& from, const Vec3& to);
		Quaternion(float angle, const Vec3 & axis);

		void setAngleAxis(float angle, const Vec3& axis);
		void setYawPitchRoll(float pitch, float roll, float yaw);
		void setRotation(const Vec3& from, const Vec3& to);

		Vec3 rotate(const Vec3& v) const;
		Vec3 operator*(const Vec3& v);
		Vec3 toVec3() const;

		void addAngle(float angle);

		void addRotation(float angle, const Vec3& axis);
		void addRotation(float pitch, float roll, float yaw);
		void addRotation(const Quaternion& q);
		Quaternion operator*(const Quaternion& q);
		Quaternion& operator*=(const Quaternion& q);
		Mat3 operator*(const Mat3& m);
		Mat4 operator*(const Mat4& m);

		Quaternion operator*(float a);
		Quaternion operator/(float a);
		Quaternion& operator*=(float a);
		Quaternion& operator/=(float a);

		Vec3 getAxis() const;
		float getAngle() const;
		Quaternion getConjugate() const;
		Quaternion getInverse() const;

		Quaternion& conjugate();
		Quaternion& inverse();
		Quaternion& normalize();

		float length() const;
		float length2() const;

		void calculateMatrix();
		Mat3 getMatrix3() const;
		Mat4 getMatrix4() const;
	private:
		void init(float s, float x, float y, float z);

		Mat3 matrix;
	};
}

#endif
