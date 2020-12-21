#include "canpch.h"
#include "Math.h"

namespace Can::Math
{
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

        return false;
    }
    bool CheckLineSegmentLineSegmentCollision(const std::array<glm::vec2, 2>& lineSegmentA, const std::array<glm::vec2, 2>& lineSegmentB, glm::vec2* intersection)
    {
		float s_numer, t_numer, denom, t;
		glm::vec2 s10 = lineSegmentA[1] - lineSegmentA[0];
		glm::vec2 s32 = lineSegmentB[0] - lineSegmentB[1];

		denom = s10.x * s32.y - s32.x * s10.y;
		if (denom == 0)
			return false; // Collinear
		bool denomPositive = denom > 0;

		glm::vec2 s02 = lineSegmentA[0] - lineSegmentB[0];

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
			glm::cos(angle) * point.x - glm::sin(angle) * point,
			glm::sin(angle) * point.x + glm::cos(angle) * point
		};
    }
	glm::vec2 RotatePointAroundPoint(const glm::vec2& p1, float angle, const glm::vec2& p2)
	{
		return glm::vec2{
			glm::cos(angle) * (p1.x - p2.x) - glm::sin(angle) * (p1.y - p2.y) + p2.x,
			glm::sin(angle) * (p1.x - p2.x) + glm::cos(angle) * (p1.y - p2.y) + p2.y
		};
	}
}