#include "canpch.h"
#include "Math.h"

namespace Can::Math
{
	std::vector<glm::vec3> GetCubicCurveSamples(const std::array<glm::vec3, 4>& vs, float preferedLength, std::vector<float>& ts)
	{
		glm::vec3 startP = vs[0];

		while (true)
		{
			float l = glm::length(vs[3] - startP[0]);

			float prevT = ts[ts.size() - 1];
			float t = 1.0f;

			while (l > preferedLength * 0.8f)
			{
				t = (t + prevT) * 0.5f;
				glm::vec3 p = Math::CubicCurve<float>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT);
			while (l > preferedLength * 0.8f)
			{
				t -= (t - prevT) * 0.1f;
				glm::vec3 p = Math::CubicCurve<float>(vs, t);
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
				startP = Math::CubicCurve<float>(vs, t);
			}
		}
		size_t Size = ts.size();
		std::vector<glm::vec3> points;
		points.resize(Size);
		points[0] = vs[0];
		for (size_t i = 1; i < Size - 1; i++)
			points[i] = Math::CubicCurve<float>(vs, ts[i]);
		points[Size - 1] = vs[3];

		constexpr size_t Quality = 2;
		for (size_t i = 0; i < Quality; i++)
		{
			float avgLength = 0.0f;
			for (size_t i = 0; i < Size - 1; i++)
				avgLength += glm::length(points[i] - points[i + 1]);
			avgLength /= (Size - 1.0f);

			for (size_t i = 1; i < Size - 1; i++)
			{
				float l = glm::length(points[i] - points[i - 1]);
				float ratio = (l + avgLength) / (l * 2.0f);
				if (ratio <= 0.0f)
					continue;
				float nextT = (ts[i] - ts[i - 1]) * ratio + ts[i - 1];
				if (nextT >= ts[i + 1])
					continue;
				ts[i] = nextT;
				points[i] = CubicCurve(vs, ts[i]);
			}
			float l = glm::length(points[Size - 1] - points[Size - 2]);
			float ratio = (l + avgLength) / (l * 2.0f);
			ts[Size - 2] = 1.0f - (1.0f - ts[Size - 2]) * ratio;
			points[Size - 2] = CubicCurve(vs, ts[Size - 2]);
		}
		return points;
	}
	std::vector<float> GetCubicCurveSampleTs(const std::array<glm::vec3, 4>& vs, float preferedLength)
	{
		std::vector<float> result{ 0.0f };
		glm::vec3 startP = vs[0];

		while (true)
		{
			float l = glm::length(vs[3] - startP[0]);

			float prevT = result[result.size() - 1];
			float t = 1.0f;

			while (l > preferedLength * 0.8f)
			{
				t = (t + prevT) * 0.5f;
				glm::vec3 p = Math::CubicCurve<float>(vs, t);
				l = glm::length(p - startP);
			}
			t += (t - prevT);
			while (l > preferedLength * 0.8f)
			{
				t -= (t - prevT) * 0.1f;
				glm::vec3 p = Math::CubicCurve<float>(vs, t);
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
				startP = Math::CubicCurve<float>(vs, t);
			}
		}
		size_t Size = result.size();
		std::vector<glm::vec3> points;
		points.resize(Size);
		points[0] = vs[0];
		for (size_t i = 1; i < Size - 1; i++)
			points[i] = Math::CubicCurve<float>(vs, result[i]);
		points[Size - 1] = vs[3];

		constexpr size_t Quality = 2;
		for (size_t i = 0; i < Quality; i++)
		{
			float avgLength = 0.0f;
			for (size_t i = 0; i < Size - 1; i++)
				avgLength += glm::length(points[i] - points[i + 1]);
			avgLength /= (Size - 1.0f);

			for (size_t i = 1; i < Size - 1; i++)
			{
				float l = glm::length(points[i] - points[i - 1]);
				float ratio = (l + avgLength) / (l * 2.0f);
				if (ratio <= 0.0f)
					continue;
				float nextT = (result[i] - result[i - 1]) * ratio + result[i - 1];
				if (nextT >= result[i + 1])
					continue;
				result[i] = nextT;
				points[i] = CubicCurve(vs, result[i]);
			}
			float l = glm::length(points[Size - 1] - points[Size - 2]);
			float ratio = (l + avgLength) / (l * 2.0f);
			result[Size - 2] = 1.0f - (1.0f - result[Size - 2]) * ratio;
			points[Size - 2] = CubicCurve(vs, result[Size - 2]);
		}
		return result;
	}
	
	bool CheckPointTriangleCollision(const std::array<glm::vec2, 3>& triangleA, const glm::vec2& point)
	{
		glm::vec2 u = (triangleA[2] - triangleA[1]) - (glm::dot(triangleA[2] - triangleA[0], triangleA[2] - triangleA[1]) / glm::dot(triangleA[2] - triangleA[0], triangleA[2] - triangleA[0])) * (triangleA[2] - triangleA[0]);
		glm::vec2 v = (triangleA[1] - triangleA[0]) - (glm::dot(triangleA[1] - triangleA[2], triangleA[1] - triangleA[0]) / glm::dot(triangleA[1] - triangleA[2], triangleA[1] - triangleA[2])) * (triangleA[1] - triangleA[2]);

		float a = 1 - glm::dot(v, point - triangleA[0]) / glm::dot(v, triangleA[1] - triangleA[0]);
		float b = 1 - glm::dot(u, point - triangleA[1]) / glm::dot(u, triangleA[2] - triangleA[1]);

		return a > 0 && a < 1 && b > 0 && b < 1 && a + b < 1;
	}
	bool CheckTriangleTriangleCollision(const std::array<glm::vec2, 3>& triangleA, const std::array<glm::vec2, 3>& triangleB)
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
					std::array<glm::vec2, 2>{ triangleA[i], triangleA[(i + 1) % 3] },
					std::array<glm::vec2, 2>{ triangleB[j], triangleB[(j + 1) % 3] }
		))
					return true;


		return false;
	}
	bool CheckLineSegmentLineSegmentCollision(const std::array<glm::vec2, 2>& lineSegmentA, const std::array<glm::vec2, 2>& lineSegmentB, glm::vec2* intersection)
	{
		float s_numer, t_numer, denom, t;
		glm::vec2 s10 = lineSegmentA[0] - lineSegmentA[1];
		glm::vec2 s32 = lineSegmentB[0] - lineSegmentB[1];

		denom = s10.x * s32.y - s32.x * s10.y;
		if (denom == 0)
			return false; // Collinear
		bool denomPositive = denom > 0;

		glm::vec2 s02 = lineSegmentB[0] - lineSegmentA[0];

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

	glm::vec2 RotatePoint(const glm::vec2& point, float angle)
	{
		return glm::vec2{
			glm::cos(angle) * point.x - glm::sin(angle) * point.y,
			glm::sin(angle) * point.x + glm::cos(angle) * point.y
		};
	}
	glm::vec2 RotatePointAroundPoint(const glm::vec2& p1, float angle, const glm::vec2& p2)
	{
		return glm::vec2{
			glm::cos(angle) * (p1.x - p2.x) - glm::sin(angle) * (p1.y - p2.y) + p2.x,
			glm::sin(angle) * (p1.x - p2.x) + glm::cos(angle) * (p1.y - p2.y) + p2.y
		};
	}
	
	std::array<std::array<glm::vec2, 3>, 2> GetBoundingBoxOfBezierCurve(const std::array<glm::vec3, 4>& Points, float halfRoadWidth)
	{
		glm::vec2 A1{ Points[0].x, Points[0].z };
		glm::vec2 B1{ Points[1].x, Points[1].z };
		glm::vec2 C1{ Points[2].x, Points[2].z };
		glm::vec2 D1{ Points[3].x, Points[3].z };

		glm::vec2 AB1 = halfRoadWidth * glm::normalize(B1 - A1);
		glm::vec2 DC1 = halfRoadWidth * glm::normalize(C1 - D1);

		AB1 = glm::vec2{ -AB1.y , AB1.x };
		DC1 = glm::vec2{ -DC1.y , DC1.x };

		glm::vec2 A11 = A1 + AB1;
		glm::vec2 A12 = A1 - AB1;
		glm::vec2 B11 = B1 + AB1;
		glm::vec2 B12 = B1 - AB1;
		glm::vec2 C11 = C1 + DC1;
		glm::vec2 C12 = C1 - DC1;
		glm::vec2 D11 = D1 + DC1;
		glm::vec2 D12 = D1 - DC1;

		auto [mins, maxs] = Math::GetMinsAndMaxs(std::array<glm::vec2, 8>{A11, A12, B11, B12, C11, C12, D11, D12});

		glm::vec2 P1{ mins.x, mins.y };
		glm::vec2 P2{ mins.x, maxs.y };
		glm::vec2 P3{ maxs.x, maxs.y };
		glm::vec2 P4{ maxs.x, mins.y };

		return std::array<std::array<glm::vec2, 3>, 2>{
			std::array<glm::vec2, 3>{P1, P2, P3},
				std::array<glm::vec2, 3>{P1, P3, P4}
		};
	}
}