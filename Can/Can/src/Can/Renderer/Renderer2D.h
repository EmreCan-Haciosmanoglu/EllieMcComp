#pragma once
#include "Can/Camera/Camera.h"
#include "Texture.h"

#undef DrawText
namespace Can
{
	struct DrawQuadParameters
	{
		glm::vec3 Position;
		glm::vec2 Size{ 1.0f, 1.0f };
		float RotationInRadians;
		glm::vec4 TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> texture = nullptr;
		const std::array<float, 4>& trim{ 0.0f, 1.0f, 1.0f, 0.0f };
		float radius = 0.0f;
		uint8_t radiusQuality = 1; // times 2
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera::OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const DrawQuadParameters& parameters);
		static void DrawQuad(const glm::mat4& transform, const DrawQuadParameters& parameters);

		static void DrawRoundedQuad(const DrawQuadParameters& parameters);
		static void DrawRoundedQuad(const glm::mat4& transform, const DrawQuadParameters& parameters);

		static void DrawText(const std::string& text, const glm::vec3& position, const glm::vec4& color, float zoomLevel = 1.0f);
	};
}