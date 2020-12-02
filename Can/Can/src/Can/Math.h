#pragma once


namespace Can::Math
{

	template <typename T>
	class Vector2
	{
	public:
		Vector2(T x, T y) : x(x), y(y) {}
		Vector2(const glm::vec2& other) :x(other.x), y(other.y) {}

		Vector2<T> operator+ (const Vector2<T>& right) const
		{
			return Vector2(this->x + right.x, this->.y + right.y);
		}
		Vector2<T> operator- (const Vector2<T>& right) const
		{
			return Vector2(this->.x - right.x, this->.y - right.y);
		}
		Vector2<T> operator* (const Vector2<T>& right) const
		{
			return Vector2(this->.x * right.x, this->.y * right.y);
		}
		Vector2<T> operator/ (const Vector2<T>& right) const
		{
			return Vector2(this->.x / right.x, this->.y / right.y);
		}

		Vector2<T> operator+ (T right)
		{
			return Vector2(this->.x + right, this->.y + right);
		}
		Vector2<T> operator- (T right)
		{
			return Vector2(this->.x - right, this->.y - right);
		}
		Vector2<T> operator* (T right)
		{
			return Vector2(this->.x * right, this->.y * right);
		}
		Vector2<T> operator/ (T right)
		{
			return Vector2(this->.x / right, this->.y / right);
		}

		operator glm::vec2() const
		{
			return glm::vec2{ x, y };
		}
	public:
		T x, y;
	};

	template <typename T>
	class Vector3
	{
	public:
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector3(const glm::vec3& other) :x(other.x), y(other.y), z(other.z) {}

		Vector3<T> operator+ (const Vector3<T>& right) const
		{
			return Vector3(this->x + right.x, this->y + right.y, this->z + right.z);
		}
		Vector3<T> operator- (const Vector3<T>& right) const
		{
			return Vector3(this->x - right.x, this->y - right.y, this->z - right.z);
		}
		Vector3<T> operator* (const Vector3<T>& right) const
		{
			return Vector3(this->x * right.x, this->y * right.y, this->z * right.z);
		}
		Vector3<T> operator/ (const Vector3<T>& right) const
		{
			return Vector3(this->x / right.x, this->y / right.y, this->z / right.z);
		}

		Vector3<T> operator+ (T right)
		{
			return Vector3(this->x + right, this->y + right, this->z + right);
		}
		Vector3<T> operator- (T right)
		{
			return Vector3(this->x - right, this->y - right, this->z - right);
		}
		Vector3<T> operator* (T right)
		{
			return Vector3(this->x * right, this->y * right, this->z * right);
		}
		Vector3<T> operator/ (T right)
		{
			return Vector3(this->x / right, this->y / right, this->z / right);
		}

		operator glm::vec3() const
		{
			return glm::vec3{ x, y, z };
		}

	public:
		T x, y, z;
	};

	template<typename T>
	Vector2<T> Lerp(Vector2<T> v1, Vector2<T>v2, T percentage)
	{
		return v1 + (v2 - v1) * percentage;
	}

	template<typename T>
	T Lerp(T a, T b, T percentage)
	{
		return a + (b - a) * percentage;
	}

	template<typename T>
	Vector3<T> Lerp(Vector3<T> v1, Vector3<T>v2, T percentage)
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
	Vector3<T> QuadraticCurve(Vector3<T> v1, Vector3<T>v2, Vector3<T>v3, T percentage)
	{
		Vector3 p0 = Lerp(v1, v2, percentage);
		Vector3 p1 = Lerp(v2, v3, percentage);
		return Lerp(p0, p1, percentage);
	}

	template<typename T>
	Vector2<T> CubicCurve(Vector2<T> v1, Vector2<T>v2, Vector2<T>v3, Vector2<T>v4, T percentage)
	{
		Vector2 p0 = QuadraticCurve(v1, v2, v3, percentage);
		Vector2 p1 = QuadraticCurve(v2, v3, v4, percentage);
		return Lerp(p0, p1, percentage);

		/*Vector2 v =
			v1 +
			(v2 - v1) * t * 3 +
			(v1 - 2 * v2 + v3) * t * t * 3 +
			(v4 - v1 + (v2 - v3) * 3) * t * t * t;
		*/
	}

	template<typename T>
	Vector3<T> CubicCurve(Vector3<T> v1, Vector3<T>v2, Vector3<T>v3, Vector3<T>v4, T percentage)
	{
		Vector3 p0 = QuadraticCurve(v1, v2, v3, percentage);
		Vector3 p1 = QuadraticCurve(v2, v3, v4, percentage);
		return Lerp(p0, p1, percentage);
	}

	template<typename T>
	Vector3<T> CubicCurve(const std::array<glm::vec3, 4>& vs, T percentage)
	{
		Vector3 p0 = QuadraticCurve(
			Vector3{ vs[0].x, vs[0].y, vs[0].z },
			Vector3{ vs[1].x, vs[1].y, vs[2].z },
			Vector3{ vs[2].x, vs[2].y, vs[2].z },
			percentage
		);
		Vector3 p1 = QuadraticCurve(
			Vector3{ vs[1].x, vs[1].y, vs[1].z },
			Vector3{ vs[2].x, vs[2].y, vs[2].z },
			Vector3{ vs[3].x, vs[3].y, vs[3].z },
			percentage
		);
		return Lerp(p0, p1, percentage);
	}

}