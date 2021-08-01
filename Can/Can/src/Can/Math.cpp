#include "canpch.h"
#include "Math.h"

namespace Can::Math
{
	std::vector<v3> GetCubicCurveSamples(const std::array<v3, 4>& vs, f32 preferedLength, std::vector<f32>& ts)
	{
		v3 startP = vs[0];

		while (true)
		{
			f32 l = glm::length(vs[3] - startP[0]);

			f32 prevT = ts[ts.size() - 1];
			f32 t = 1.0f;

			while (l > preferedLength * 0.8f)
			{
				t = (t + prevT) * 0.5f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT);
			while (l > preferedLength * 0.8f)
			{
				t -= (t - prevT) * 0.1f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT) * 0.1f;

			if (t > 0.99f && l < preferedLength * 0.5f)
			{
				ts.pop_back();
				ts.push_back(1.0f);
				break;
			}
			else
			{
				ts.push_back(t);
				startP = Math::CubicCurve<f32>(vs, t);
			}
		}
		size_t Size = ts.size();
		std::vector<v3> points;
		points.resize(Size);
		points[0] = vs[0];
		for (size_t i = 1; i < Size - 1; i++)
			points[i] = Math::CubicCurve<f32>(vs, ts[i]);
		points[Size - 1] = vs[3];

		if (Size < 2) return {vs[0], vs[3]};
		
		constexpr size_t Quality = 2;
		for (size_t i = 0; i < Quality; i++)
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
				f32 nextT = (ts[i] - ts[i - 1]) * ratio + ts[i - 1];
				if (nextT >= ts[i + 1])
					continue;
				ts[i] = nextT;
				points[i] = CubicCurve(vs, ts[i]);
			}
			f32 l = glm::length(points[Size - 1] - points[Size - 2]);
			f32 ratio = (l + avgLength) / (l * 2.0f);
			ts[Size - 2] = 1.0f - (1.0f - ts[Size - 2]) * ratio;
			points[Size - 2] = CubicCurve(vs, ts[Size - 2]);
		}
		return points;
	}
	std::vector<f32> GetCubicCurveSampleTs(const std::array<v3, 4>& vs, f32 preferedLength)
	{
		std::vector<f32> result{ 0.0f };
		v3 startP = vs[0];

		while (true)
		{
			f32 l = glm::length(vs[3] - startP[0]);

			f32 prevT = result[result.size() - 1];
			f32 t = 1.0f;

			while (l > preferedLength * 0.8f)
			{
				t = (t + prevT) * 0.5f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT);
			while (l > preferedLength * 0.8f)
			{
				t -= (t - prevT) * 0.1f;
				v3 p = Math::CubicCurve<f32>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT) * 0.1f;

			if (t > 0.99f && l < preferedLength * 0.5f)
			{
				result.pop_back();
				result.push_back(1.0f);
				break;
			}
			else
			{
				result.push_back(t);
				startP = Math::CubicCurve<f32>(vs, t);
			}
		}
		size_t Size = result.size();
		std::vector<v3> points;
		points.resize(Size);
		points[0] = vs[0];
		for (size_t i = 1; i < Size - 1; i++)
			points[i] = Math::CubicCurve<f32>(vs, result[i]);
		points[Size - 1] = vs[3];

		if (Size < 2) return result;

		constexpr size_t Quality = 2;
		for (size_t i = 0; i < Quality; i++)
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
	
	bool CheckPointTriangleCollision(const std::array<v2, 3>& triangleA, const v2& point)
	{
		v2 u = (triangleA[2] - triangleA[1]) - (glm::dot(triangleA[2] - triangleA[0], triangleA[2] - triangleA[1]) / glm::dot(triangleA[2] - triangleA[0], triangleA[2] - triangleA[0])) * (triangleA[2] - triangleA[0]);
		v2 v = (triangleA[1] - triangleA[0]) - (glm::dot(triangleA[1] - triangleA[2], triangleA[1] - triangleA[0]) / glm::dot(triangleA[1] - triangleA[2], triangleA[1] - triangleA[2])) * (triangleA[1] - triangleA[2]);

		f32 a = 1 - glm::dot(v, point - triangleA[0]) / glm::dot(v, triangleA[1] - triangleA[0]);
		f32 b = 1 - glm::dot(u, point - triangleA[1]) / glm::dot(u, triangleA[2] - triangleA[1]);

		return a > 0 && a < 1 && b > 0 && b < 1 && a + b < 1;
	}
	bool CheckTriangleTriangleCollision(const std::array<v2, 3>& triangleA, const std::array<v2, 3>& triangleB)
	{
		for (size_t i = 0; i < 3; i++)
			if (CheckPointTriangleCollision(triangleA, triangleB[i]))
				return true;

		for (size_t i = 0; i < 3; i++)
			if (CheckPointTriangleCollision(triangleB, triangleA[i]))
				return true;

		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				if (CheckLineSegmentLineSegmentCollision(
					std::array<v2, 2>{ triangleA[i], triangleA[(i + 1) % 3] },
					std::array<v2, 2>{ triangleB[j], triangleB[(j + 1) % 3] }
		))
					return true;


		return false;
	}
	bool CheckPolygonPointCollision(const std::vector<std::array<v2, 3>>& polygon, const v2& point)
	{
		u64 count = polygon.size();
		for (size_t i = 0; i < count; i++)
			if (CheckPointTriangleCollision(polygon[i], point))
				return true;
		return false;
	}
	bool CheckLineSegmentLineSegmentCollision(const std::array<v2, 2>& lineSegmentA, const std::array<v2, 2>& lineSegmentB, v2* intersection)
	{
		f32 s_numer, t_numer, denom, t;
		v2 s10 = lineSegmentA[0] - lineSegmentA[1];
		v2 s32 = lineSegmentB[0] - lineSegmentB[1];

		denom = s10.x * s32.y - s32.x * s10.y;
		if (denom == 0)
			return false; // Collinear
		bool denomPositive = denom > 0;

		v2 s02 = lineSegmentB[0] - lineSegmentA[0];

		s_numer = s10.x * s02.y - s10.y * s02.x;
		if ((s_numer < 0) == denomPositive)
			return false; // No collision

		t_numer = s32.x * s02.y - s32.y * s02.x;
		if ((t_numer < 0) == denomPositive)
			return false; // No collision

		if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
			return false; // No collision
		// Collision detected
		t = t_numer / denom;
		if (intersection)
			(*intersection) = lineSegmentA[0] + (t * s10);
		return true;
	}

	v2 RotatePoint(const v2& point, f32 angle)
	{
		return v2{
			glm::cos(angle) * point.x - glm::sin(angle) * point.y,
			glm::sin(angle) * point.x + glm::cos(angle) * point.y
		};
	}
	v2 RotatePointAroundPoint(const v2& p1, f32 angle, const v2& p2)
	{
		return v2{
			glm::cos(angle) * (p1.x - p2.x) - glm::sin(angle) * (p1.y - p2.y) + p2.x,
			glm::sin(angle) * (p1.x - p2.x) + glm::cos(angle) * (p1.y - p2.y) + p2.y
		};
	}
	
	std::array<std::array<v2, 3>, 2> GetBoundingBoxOfBezierCurve(const std::array<v3, 4>& Points, f32 halfRoadWidth)
	{
		v2 A1{ Points[0].x, Points[0].y };
		v2 B1{ Points[1].x, Points[1].y };
		v2 C1{ Points[2].x, Points[2].y };
		v2 D1{ Points[3].x, Points[3].y };

		v2 AB1 = halfRoadWidth * glm::normalize(B1 - A1);
		v2 DC1 = halfRoadWidth * glm::normalize(C1 - D1);

		AB1 = v2{ -AB1.y , AB1.x };
		DC1 = v2{ -DC1.y , DC1.x };

		v2 A11 = A1 + AB1;
		v2 A12 = A1 - AB1;
		v2 B11 = B1 + AB1;
		v2 B12 = B1 - AB1;
		v2 C11 = C1 + DC1;
		v2 C12 = C1 - DC1;
		v2 D11 = D1 + DC1;
		v2 D12 = D1 - DC1;

		auto [mins, maxs] = Math::GetMinsAndMaxs(std::array<v2, 8>{A11, A12, B11, B12, C11, C12, D11, D12});

		v2 P1{ mins.x, mins.y };
		v2 P2{ mins.x, maxs.y };
		v2 P3{ maxs.x, maxs.y };
		v2 P4{ maxs.x, mins.y };

		return std::array<std::array<v2, 3>, 2>{
			std::array<v2, 3>{P1, P2, P3},
				std::array<v2, 3>{P1, P3, P4}
		};
	}

	v3 RayPlaneIntersection(const v3& X, const v3& v, const v3& C, const v3& n)
	{
		v3 w = C - X;
		f32 k = glm::dot(w, n) / glm::dot(v, n);
		v3 result = X + k * v;
		if (isnan(result.x))
			result.x = (int)(&result.x);
		if (isnan(result.y))
			result.y = (int)(&result.y);
		if (isnan(result.z))
			result.z = (int)(&result.z);
		return result;
	}
}
