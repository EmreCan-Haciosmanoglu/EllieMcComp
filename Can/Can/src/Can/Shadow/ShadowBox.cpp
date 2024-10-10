#include "canpch.h"
#include "ShadowBox.h"

namespace Can
{
	ShadowBox::ShadowBox( const Perspective_Camera_Controller* cameraController)
		: cameraController(cameraController)
	{
		CalculateWidthsAndHeights();
	}

	void ShadowBox::Update()
	{
		auto& camera = cameraController->camera;
		glm::vec3 forward = glm::normalize(camera.forward);

		glm::vec3 toFar = forward * SHADOW_DISTANCE;
		glm::vec3 toNear = forward * camera.near_clip_plane;

		glm::vec3 centerFar = toFar + camera.position;
		glm::vec3 centerNear = toNear + camera.position;

		glm::vec4 points[8] = {};
		CalculateFrastumVertices(centerNear, centerFar, points);

		minVal = glm::vec3{ points[0].x, points[0].y, points[0].z };
		maxVal = glm::vec3{ points[0].x, points[0].y, points[0].z };

		for (size_t i = 1; i < 8; i++)
		{
			minVal.x = std::min(minVal.x, points[i].x);
			minVal.y = std::min(minVal.y, points[i].y);
			minVal.z = std::min(minVal.z, points[i].z);

			maxVal.x = std::max(maxVal.x, points[i].x);
			maxVal.y = std::max(maxVal.y, points[i].y);
			maxVal.z = std::max(maxVal.z, points[i].z);
		}
	}

	void ShadowBox::CalculateWidthsAndHeights()
	{
		farSize.x = SHADOW_DISTANCE * glm::tan(glm::radians(cameraController->camera.field_of_view_angle));
		nearSize.x = cameraController->camera.near_clip_plane * glm::tan(glm::radians(cameraController->camera.field_of_view_angle));

		farSize.y = farSize.x / cameraController->camera.aspect_ratio;
		nearSize.y = nearSize.x / cameraController->camera.aspect_ratio;
	}

	void ShadowBox::CalculateFrastumVertices(const glm::vec3& centerNear, const glm::vec3& centerFar, glm::vec4* output)
	{
		glm::vec3 up = cameraController->camera.up;
		glm::vec3 right = cameraController->camera.right;


		glm::vec3 farTop = centerFar + up * farSize.y;
		glm::vec3 farBottom = centerFar - up * farSize.y;
		glm::vec3 nearTop = centerNear + up * nearSize.y;
		glm::vec3 nearBottom = centerNear - up * nearSize.y;

		output[0] = v4(farTop + right * farSize.x, 1.0f);
		output[1] = v4(farTop - right * farSize.x, 1.0f);
		output[2] = v4(farBottom + right * farSize.x, 1.0f);
		output[3] = v4(farBottom - right * farSize.x, 1.0f);
		output[4] = v4(nearTop + right * nearSize.x, 1.0f);
		output[5] = v4(nearTop - right * nearSize.x, 1.0f);
		output[6] = v4(nearBottom + right * nearSize.x, 1.0f);
		output[7] = v4(nearBottom - right * nearSize.x, 1.0f);

		output[0] = CalculateLightSpaceFrustumCorner(output[4], output[0]);
		output[1] = CalculateLightSpaceFrustumCorner(output[5], output[1]);
		output[2] = CalculateLightSpaceFrustumCorner(output[6], output[2]);
		output[3] = CalculateLightSpaceFrustumCorner(output[7], output[3]);
		output[4] = lightViewMatrix * output[4];
		output[5] = lightViewMatrix * output[5];
		output[6] = lightViewMatrix * output[6];
		output[7] = lightViewMatrix * output[7];
	}
}