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
			Vector3{ vs[1].x, vs[1].y, vs[1].z },
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

	template<typename T>
	Vector3<T> QuadraticCurve(const std::array<glm::vec3, 3>& vs, T percentage)
	{
		return QuadraticCurve(
			Vector3{ vs[0].x, vs[0].y, vs[0].z },
			Vector3{ vs[1].x, vs[1].y, vs[1].z },
			Vector3{ vs[2].x, vs[2].y, vs[2].z },
			percentage
		);
	}

	template <int Size, int Quality>
	std::array<float, Size> GetCubicCurveSampleTs(const std::array<glm::vec3, 4>& vs)
	{
		std::array<float, Size> result;
		result[0] = 0.0f;
		result[Size-1] = 1.0f;

		std::array<glm::vec3, Size> points;
		points[0] = vs[0];
		points[Size-1] = vs[3];

		for (size_t i = 1; i < Size - 1; i++)
		{
			float t = i / (Size - 1.0f);
			result[i] = t;
			points[i] = CubicCurve(vs, t);
		}

		for (size_t k = 0; k < Quality; k++)
		{
			float avgLength = 0.0f;
			for (size_t i = 0; i < Size - 1; i++)
				avgLength += glm::length(points[i] - points[i + 1]);
			avgLength /= (Size - 1.0f);

			for (size_t i = 1; i < Size - 1; i++)
			{
				float l = glm::length(points[i] - points[i - 1]);
				float ratio = (l + avgLength) / (l * 2.0f);
				result[i] = (result[i] - result[i - 1]) * ratio + result[i - 1];
				points[i] = CubicCurve(vs, result[i]);
			}
			float l = glm::length(points[Size - 1] - points[Size - 2]);
			float ratio = (l + avgLength) / (l * 2.0f);
			result[Size - 2] = 1.0f - (1.0f - result[Size - 2]) * ratio;
			points[Size - 2] = CubicCurve(vs, result[Size - 2]);
		}

		return result;
	}

	template <int Size, int Quality>
	std::array<glm::vec2, Size> GetCubicCurveSamples(const std::array<glm::vec2, 4>& vs)
	{
		std::array<glm::vec2, Size> result;
		return result;
	}

	bool CheckPointTriangleCollision(const std::array<glm::vec2, 3>& triangleA, const glm::vec2& point);

	bool CheckTriangleTriangleCollision(const std::array<glm::vec2, 3>& triangleA, const std::array<glm::vec2, 3>& triangleB);

	template <int countA, int countB>
	bool CheckPolygonCollision(const std::array<std::array<glm::vec2, 3>, countA>& polygonA, const std::array<std::array<glm::vec2, 3>, countB>& polygonB)
	{
		for (size_t i = 0; i < countA; i++)
			for (size_t j = 0; j < countB; j++)
				if (CheckTriangleTriangleCollision(polygonA[i], polygonB[j]))
					return true;

		return false;
	}

	bool CheckLineSegmentLineSegmentCollision(const std::array<glm::vec2, 2>& lineSegmentA, const std::array<glm::vec2, 2>& lineSegmentB, glm::vec2* intersection = nullptr);

	template <int countA, int countB>
	bool CheckLineSegmentListLineSegmentListCollision(const std::array<std::array<glm::vec2, 2>, countA>& lineSegmentListA, const std::array<std::array<glm::vec2, 2>, countB>& lineSegmentListB)
	{
		for (size_t i = 0; i < countA; i++)
			for (size_t j = 0; j < countB; j++)
				if (CheckLineSegmentLineSegmentCollision(lineSegmentListA[i], lineSegmentListB[j]))
					return true;

		return false;
	}

	glm::vec2 RotatePoint(const glm::vec2& point, float angle);
	glm::vec2 RotatePointAroundPoint(const glm::vec2& p1, float angle, const glm::vec2& p2);
}