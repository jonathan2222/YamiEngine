#ifndef YAMI_VECTOR_2_H
#define YAMI_VECTOR_2_H

namespace ym
{
	class Vec3;
	class Vec4;
	class Vec2
	{
	public:
		static Vec2 VEC2_X;
		static Vec2 VEC2_Y;
		static Vec2 VEC2_ONES;
		static Vec2 VEC2_ZEROS;

		union
		{
			struct { float x, y; };
			struct { float r, g; };
			struct { float s, t; };
		};
		
	private:
		typedef float Vec2::* const vec[2];
		static const vec v;

	public:
		// Set to zero.
		Vec2();
		// Fill it with value
		Vec2(float value);
		Vec2(float x, float y);
		Vec2(const Vec2& v);
		Vec2(const Vec3& v);
		Vec2(const Vec4& v);

		float& operator[](size_t i);
		const float& operator[](size_t i) const;

		bool operator==(const Vec2& v);
		bool operator!=(const Vec2& v);

		Vec2& operator=(const Vec2& v);

		Vec2& operator+=(const Vec2& v);
		Vec2& operator-=(const Vec2& v);
		Vec2& operator+=(float scalar);
		Vec2& operator-=(float scalar);
		Vec2& operator*=(float scalar);
		Vec2& operator/=(float scalar);

		Vec2 operator+(const Vec2& v) const;
		Vec2 operator-(const Vec2& v) const;
		Vec2 operator+(float scalar) const;
		Vec2 operator-(float scalar) const;
		Vec2 operator*(float scalar) const;
		Vec2 operator/(float scalar) const;

		Vec2& operator++();
		Vec2& operator--();
		Vec2 operator++(int a) const;
		Vec2 operator--(int a) const;
		Vec2 operator-() const;

		int length() const;
		Vec3 toVec3() const;
		Vec4 toVec4() const;
	};
}

inline ym::Vec2 operator+(float scalar, const ym::Vec2& v) { return v + scalar; }
inline ym::Vec2 operator-(float scalar, const ym::Vec2& v) { return v - scalar; }
inline ym::Vec2 operator*(float scalar, const ym::Vec2& v) { return v * scalar; }

#endif
