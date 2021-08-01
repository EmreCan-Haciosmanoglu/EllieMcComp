#pragma once
#include <glm/glm.hpp>
#include "Can/Camera/Perspective_Camera_Controller.h"
namespace Can
{
#define OFFSET 10.0f
#define UP (glm::vec3(0.0f, 1.0f, 0.0f))
#define FORWARD (glm::vec3(0.0f, 0.0f, -1.0f))
#define SHADOW_DISTANCE 10.0f

	class ShadowBox
	{
	public:
		ShadowBox(const glm::mat4& mat, const Perspective_Camera_Controller* cameraController);

		void Update();

		void CalculateWidthsAndHeights();
		void CalculateFrastumVertices(
			const glm::vec3& forward,
			const glm::vec3& centerNear,
			const glm::vec3& centerFar,
			glm::vec4* output
		);
		glm::vec4 CalculateLightSpaceFrustumCorner(
			const glm::vec3& point,
			const glm::vec3& direction,
			float length)
		{
			glm::vec4 newPos = glm::vec4((point + direction * length), 1.0f);
			newPos.y = (std::max)(newPos.y, -1.0f);
			return lightViewMatrix * newPos;
		}

		glm::mat4 CalculateCameraRotation();

		inline glm::vec3 GetCenter()
		{
			glm::vec3 centerInLightSpace = (maxVal + minVal) / 2.0f;
			glm::mat4 invertedLightSpace = glm::inverse(lightViewMatrix);
			glm::vec4 centerInWorldSpace = invertedLightSpace * glm::vec4(centerInLightSpace, 1.0f);
			return glm::vec3{ centerInWorldSpace.x, centerInWorldSpace.y, centerInWorldSpace.z };
		}

		inline float GetWidth() const { return maxVal.x - minVal.x; }
		inline float GetHeight() const { return maxVal.y - minVal.y; }
		inline float GetLength() const { return maxVal.z - minVal.z; }

		inline const glm::vec3& GetMins() const { return minVal; }
		inline const glm::vec3& GetMaxs() const { return maxVal; }

	private:
		glm::vec3 minVal = glm::vec3(0.0f);
		glm::vec3 maxVal = glm::vec3(0.0f);

		glm::mat4 lightViewMatrix = glm::mat4(1.0f);

		glm::vec2 farSize = glm::vec2(1.0f);
		glm::vec2 nearSize = glm::vec2(1.0f);

		const Perspective_Camera_Controller* cameraController;
	};
}