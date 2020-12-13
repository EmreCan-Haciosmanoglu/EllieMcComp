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
}