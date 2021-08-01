#include "canpch.h"
#include "ShadowBox.h"

namespace Can
{
	ShadowBox::ShadowBox(const glm::mat4& mat, const Perspective_Camera_Controller* cameraController)
		: lightViewMatrix(mat)
		, cameraController(cameraController)
	{
		CalculateWidthsAndHeights();
	}

	void ShadowBox::Update()
	{

		glm::vec3 camRot = cameraController->camera.rotation;
		glm::vec3 forward = {
			-glm::sin(glm::radians(camRot.y)) * glm::cos(glm::radians(camRot.x)),
			glm::sin(glm::radians(camRot.x)),
			-glm::cos(glm::radians(camRot.x)) * glm::cos(glm::radians(camRot.y))
		};

		glm::vec3 toFar = glm::normalize(forward) * SHADOW_DISTANCE;
		glm::vec3 toNear = glm::normalize(forward) * cameraController->camera.near_clip_plane;

		glm::vec3 centerFar = toFar + cameraController->camera.position;
		glm::vec3 centerNear = toNear + cameraController->camera.rotation;

		glm::vec4 points[8] = {};
		CalculateFrastumVertices(forward, centerNear, centerFar, points);

		minVal = glm::vec3{ points[0].x,points[0].y,points[0].z };
		maxVal = glm::vec3{ points[0].x,points[0].y,points[0].z };

		for (size_t i = 1; i < 8; i++)
		{
			minVal.x = (std::min)(minVal.x, points[i].x);
			minVal.y = (std::min)(minVal.y, points[i].y);
			minVal.z = (std::min)(minVal.z, points[i].z);

			maxVal.x = (std::max)(maxVal.x, points[i].x);
			maxVal.y = (std::max)(maxVal.y, points[i].y);
			maxVal.z = (std::max)(maxVal.z, points[i].z);
		}
		maxVal.z += OFFSET;
	}

	void ShadowBox::CalculateWidthsAndHeights()
	{
		farSize.x = SHADOW_DISTANCE * glm::tan(glm::radians(cameraController->camera.field_of_view_angle));
		nearSize.x = cameraController->camera.near_clip_plane * glm::tan(glm::radians(cameraController->camera.field_of_view_angle));

		farSize.y = farSize.x / cameraController->camera.aspect_ratio;
		nearSize.y = nearSize.x / cameraController->camera.aspect_ratio;
	}

	void ShadowBox::CalculateFrastumVertices(const glm::vec3& forward, const glm::vec3& centerNear, const glm::vec3& centerFar, glm::vec4* output)
	{
		glm::mat4 rot = CalculateCameraRotation();
		glm::vec3 up = rot * glm::vec4(UP,1.0f);
		glm::vec3 right = glm::cross(forward, up);


		glm::vec3 farTop = centerFar + up * farSize.y;
		glm::vec3 farBottom = centerFar - up * farSize.y;
		glm::vec3 nearTop = centerNear + up * nearSize.y;
		glm::vec3 nearBottom = centerNear - up * nearSize.y;

		output[0] = CalculateLightSpaceFrustumCorner(farTop, right, farSize.x);
		output[1] = CalculateLightSpaceFrustumCorner(farTop, -right, farSize.x);
		output[2] = CalculateLightSpaceFrustumCorner(farBottom, right, farSize.x);
		output[3] = CalculateLightSpaceFrustumCorner(farBottom, -right, farSize.x);
		output[4] = CalculateLightSpaceFrustumCorner(nearTop, right, nearSize.x);
		output[5] = CalculateLightSpaceFrustumCorner(nearTop, -right, nearSize.x);
		output[6] = CalculateLightSpaceFrustumCorner(nearBottom, right, nearSize.x);
		output[7] = CalculateLightSpaceFrustumCorner(nearBottom, -right, nearSize.x);
	}

	glm::mat4 ShadowBox::CalculateCameraRotation()
	{
		const glm::vec3& camRotation = cameraController->camera.rotation;
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, glm::radians(camRotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f });
		rotation = glm::rotate(rotation, glm::radians(camRotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f });
		return rotation;
	}
}