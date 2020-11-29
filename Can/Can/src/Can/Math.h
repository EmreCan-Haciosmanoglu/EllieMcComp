#pragma once


namespace Can::Math
{
	template <typename T>
	class Vector2
	{
	public:
		Vector2(T x, T y) : x(x), y(y) {}

		Vector2 operator+ (const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x + right.x, left.y + right.y);
		}
		Vector2 operator- (const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x - right.x, left.y - right.y);
		}
		Vector2 operator* (const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x * right.x, left.y * right.y);
		}
		Vector2 operator/ (const Vector2& left, const Vector2& right)
		{
			return Vector2(left.x / right.x, left.y / right.y);
		}

		Vector2 operator+ (const Vector2& left, T right)
		{
			return Vector2(left.x + right, left.y + right);
		}
		Vector2 operator- (const Vector2& left, T right)
		{
			return Vector2(left.x - right, left.y - right);
		}
		Vector2 operator* (const Vector2& left, T right)
		{
			return Vector2(left.x * right, left.y * right);
		}
		Vector2 operator/(const Vector2& left, T right)
		{
			return Vector2(left.x / right, left.y / right);
		}

	public:
		T x, y;
	};

	template<typename T>
	Vector2<T> Lerp(Vector2<T> v1, Vector2<T>v2, T percentage)
	{
		return v1 + (v2 - v1) * percentage;
	}

	template<typename T>
	Vector2<T> QuadraticCurve(Vector2<T> v1, Vector2<T>v2, Vector2<T>v3, T percentage)
	{
		Vector2 p0 = Lerp(v1, v2, percentage);
		Vector2 p1 = Lerp(v2, v3, percentage);
		return Lerp(p0, p1, percentage);
	}

	template<typename T>
	Vector2<T> CubicCurve(Vector2<T> v1, Vector2<T>v2, Vector2<T>v3, Vector2<T>v4, T percentage)
	{
		Vector2 p0 = QuadraticCurve(v1, v2, v3, percentage);
		Vector2 p1 = QuadraticCurve(v2, v3, v4, percentage);
		return Lerp(p0, p1, percentage);
	}

}