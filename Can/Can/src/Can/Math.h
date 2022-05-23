#pragma once
namespace Can::Math
{
	template <typename T>
	class Vector2
	{
	public:
		Vector2(T x, T y) : x(x), y(y) {}
		Vector2(const v2& other) :x(other.x), y(other.y) {}

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

		operator v2() const
		{
			return v2{ x, y };
		}
	public:
		T x, y;
	};

	template <typename T>
	class Vector3
	{
	public:
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector3(const v3& other) :x(other.x), y(other.y), z(other.z) {}

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

		operator v3() const
		{
			return v3{ x, y, z };
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
	Vector3<T> CubicCurve(const std::array<v3, 4>& vs, T percentage)
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
	Vector3<T> CubicCurveTangent(const std::array<v3, 4>& vs, T percentage)
	{
		return (vs[1] - vs[0]) * 3.0f +
			(vs[0] - vs[1] * 2.0f + vs[2]) * percentage * 6.0f +
			(vs[3] - vs[0] + (vs[1] - vs[2]) * 3.0f) * percentage * percentage * 3.0f;
	}

	template<typename T>
	Vector3<T> QuadraticCurve(const std::array<v3, 3>& vs, T percentage)
	{
		return QuadraticCurve(
			Vector3{ vs[0].x, vs[0].y, vs[0].z },
			Vector3{ vs[1].x, vs[1].y, vs[1].z },
			Vector3{ vs[2].x, vs[2].y, vs[2].z },
			percentage
		);
	}

	template <int Size, int Quality>
	std::array<f32, Size> GetCubicCurveSampleTs(const std::array<v3, 4>& vs)
	{
		std::array<f32, Size> result;
		result[0] = 0.0f;
		result[Size - 1] = 1.0f;

		std::array<v3, Size> points;
		points[0] = vs[0];
		points[Size - 1] = vs[3];

		for (size_t i = 1; i < Size - 1; i++)
		{
			f32 t = i / (Size - 1.0f);
			result[i] = t;
			points[i] = CubicCurve(vs, t);
		}

		for (size_t k = 0; k < Quality; k++)
		{
			f32 avgLength = 0.0f;
			for (size_t i = 0; i < Size - 1; i++)
				avgLength += glm::length(points[i] - points[i + 1]);
			avgLength /= (Size - 1.0f);

			for (size_t i = 1; i < Size - 1; i++)
			{
				f32 l = glm::length(points[i] - points[i - 1]);
				f32 ratio = (l + avgLength) / (l * 2.0f);
				if (ratio <= 0.0f)
					continue;
				f32 nextT = (result[i] - result[i - 1]) * ratio + result[i - 1];
				if (nextT >= result[i + 1])
					continue;
				result[i] = nextT;
				points[i] = CubicCurve(vs, result[i]);
			}
			f32 l = glm::length(points[Size - 1] - points[Size - 2]);
			f32 ratio = (l + avgLength) / (l * 2.0f);
			result[Size - 2] = 1.0f - (1.0f - result[Size - 2]) * ratio;
			points[Size - 2] = CubicCurve(vs, result[Size - 2]);
		}
		return result;
	}

	std::vector<v3> GetCubicCurveSamples(const std::array<v3, 4>& vs, f32 preferedLength, std::vector<f32>& ts);
	std::vector<f32> GetCubicCurveSampleTs(const std::array<v3, 4>& vs, f32 preferedLength);

	template <int Size, int Quality>
	std::array<v2, Size> GetCubicCurveSamples(const std::array<v2, 4>& vs)
	{
		std::array<v2, Size> result;
		CAN_ASSERT(false, "Incomplete!");
		return result;
	}

	bool CheckPointTriangleCollision(const std::array<v2, 3>& triangleA, const v2& point);

	bool CheckTriangleTriangleCollision(const std::array<v2, 3>& triangleA, const std::array<v2, 3>& triangleB);

	template <int countA, int countB>
	bool CheckPolygonCollision(const std::array<std::array<v2, 3>, countA>& polygonA, const std::array<std::array<v2, 3>, countB>& polygonB)
	{
		for (size_t i = 0; i < countA; i++)
			for (size_t j = 0; j < countB; j++)
				if (CheckTriangleTriangleCollision(polygonA[i], polygonB[j]))
					return true;

		return false;
	}

	template <int countA>
	bool CheckPolygonPointCollision(const std::array<std::array<v2, 3>, countA>& polygon, const v2& point)
	{
		for (size_t i = 0; i < countA; i++)
			if (CheckPointTriangleCollision(polygon[i], point))
				return true;
		return false;
	}
	bool CheckPolygonPointCollision(const std::vector<std::array<v2, 3>>& polygon, const v2& point);

	bool CheckLineSegmentLineSegmentCollision(const std::array<v2, 2>& lineSegmentA, const std::array<v2, 2>& lineSegmentB, v2* intersection = nullptr);

	template <int countA, int countB>
	bool CheckLineSegmentListLineSegmentListCollision(const std::array<std::array<v2, 2>, countA>& lineSegmentListA, const std::array<std::array<v2, 2>, countB>& lineSegmentListB)
	{
		for (size_t i = 0; i < countA; i++)
			for (size_t j = 0; j < countB; j++)
				if (CheckLineSegmentLineSegmentCollision(lineSegmentListA[i], lineSegmentListB[j]))
					return true;

		return false;
	}

	v2 RotatePoint(const v2& point, f32 angle);
	v2 RotatePointAroundPoint(const v2& p1, f32 angle, const v2& p2);

	template <int N>
	std::array<v2, 2> GetMinsAndMaxs(const std::array<v2, N>& points)
	{
		v2 mins = points[0];
		v2 maxs = points[0];
		for (size_t i = 1; i < N; i++)
		{
			mins.x = (std::min)(mins.x, points[i].x);
			mins.y = (std::min)(mins.y, points[i].y);
			maxs.x = (std::max)(maxs.x, points[i].x);
			maxs.y = (std::max)(maxs.y, points[i].y);
		}
		return{ mins, maxs };
	}

	template <int N>
	std::array<v3, 2> GetMinsAndMaxs(const std::array<v3, N>& points)
	{
		v3 mins = points[0];
		v3 maxs = points[0];
		for (size_t i = 1; i < N; i++)
		{
			mins.x = (std::min)(mins.x, points[i].x);
			mins.y = (std::min)(mins.y, points[i].y);
			mins.z = (std::min)(mins.z, points[i].z);

			maxs.x = (std::max)(maxs.x, points[i].x);
			maxs.y = (std::max)(maxs.y, points[i].y);
			maxs.z = (std::max)(maxs.z, points[i].z);
		}
		return{ mins, maxs };
	}

	std::array<std::array<v2, 3>, 2> GetBoundingBoxOfBezierCurve(const std::array<v3, 4>& Points, f32 halfRoadWidth);

	template <int Size, int Quality>
	std::array<std::array<v2, 3>, (Size - 1) * 2> GetBoundingPolygonOfBezierCurve(const std::array<v3, 4>& Points, f32 halfRoadWidth)
	{
		std::array<std::array<v2, 3>, (Size - 1) * 2> result;
		std::array<f32, Size> samples = GetCubicCurveSampleTs<Size, Quality>(Points);
		v2 p0 = (v2)Points[0];

		for (size_t i = 1; i < Size - 1; i++)
		{
			//put these P1 and p1 outside of the for-loop
			v3 P1 = CubicCurve(Points, samples[i]);
			v2 p1 = (v2)P1;

			v3 P2 = CubicCurve(Points, samples[i + 1]);
			v2 p2 = (v2)P2;

			//put this v1 outside of the for-loop
			v2 d1 = halfRoadWidth * glm::normalize(p1 - p0);
			d1 = v2{ -d1.y, d1.x };

			v2 d2 = halfRoadWidth * glm::normalize(p2 - p1);
			d2 = v2{ -d2.y, d2.x };

			result[2 * (i - 1) + 0] = { p0 + d1, p0 - d1, p1 + d2 };
			result[2 * (i - 1) + 1] = { p0 - d1, p1 + d2, p1 - d2 };

			p0 = p1;
		}
		v3 P1 = CubicCurve(Points, 1.0f);
		v2 p1 = (v2)P1;

		v2 d1 = halfRoadWidth * glm::normalize(p1 - p0);
		v2 d2 = halfRoadWidth * glm::normalize(p1 - (v2)Points[2]);

		d1 = v2{ -d1.y, d1.x };
		d2 = v2{ -d2.y, d2.x };

		result[2 * (Size - 2) + 0] = { p0 + d1, p0 - d1, p1 + d2 };
		result[2 * (Size - 2) + 1] = { p0 - d1, p1 + d2, p1 - d2 };

		return result;
	}

	v3 RayPlaneIntersection(const v3& X, const v3& v, const v3& C, const v3& n);

	std::vector<v3> get_samples_from_cubic_bezier_curve(const std::array<v3, 4>& control_points, f32 preferred_length);
	std::array<v3, 2> get_bounding_box_from_straight_bezier_curve(const std::array<v3, 4>& control_points, f32 half_width, f32 height);
	std::array<v3, 2> get_bounding_box_from_cubic_bezier_curve(const std::array<v3, 4>& control_points, f32 half_width, f32 height);
	std::vector<std::array<v3, 3>> get_bounding_polygon_from_bezier_curve(const std::array<v3, 4>& control_points, f32 half_width, f32 preferred_length);
	std::vector<std::array<v3, 3>> get_bounding_polygon_from_bezier_curve(const std::vector<v3>& samples, f32 half_width);

	bool check_bounding_box_bounding_box_collision(
		std::array<v3, 2> bounding_box_1,
		std::array<v3, 2> bounding_box_2);
	bool check_bounding_polygon_bounding_polygon_collision_with_z(
		std::vector<std::array<v3, 3>> polygon_1,
		f32 polygon_1_height,
		std::vector<std::array<v3, 3>> polygon_2,
		f32 polygon_2_height);
	bool check_triangle_triangle_collision_with_z(
		const std::array<v3, 3>& triangle_1,
		f32 triangle_1_height,
		const std::array<v3, 3>& triangle_2,
		f32 triangle_2_height);
	bool check_triangle_point_collision(
		const std::array<v3, 3>& triangle_1,
		v3 point);
	bool check_line_segment_line_segment_collision(
		const std::array<v3, 2>& line_segment_1,
		const std::array<v3, 2>& line_segment_2);

	v3 lerp(const v3& a, const v3& b, f32 t);
	f32 lerp(f32 a, f32 b, f32 t);

	v3 safe_ray_plane_intersection(v3 X, v3 v, v3 C, v3 n);
	v3 ray_plane_intersection(const v3& X, const v3& v, const v3& C, const v3& n);

	std::array<v3, 3> get_axis_of_a_triangle(const std::array<v3, 3>& triangle);
	bool check_triangle_triangle_collision_with_SAT(const std::array<v3, 3>& triangle_1, const std::array<v3, 3>& triangle_2);
}