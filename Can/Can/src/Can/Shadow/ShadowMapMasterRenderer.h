#pragma once
#include "ShadowMapEntityRenderer.h"
#include "ShadowFrameBuffer.h"
#include "ShadowBox.h"

namespace Can
{
#define SHADOW_MAP_SIZE 2048
	class ShadowMapMasterRenderer
	{
	public:
		ShadowMapMasterRenderer(Perspective_Camera_Controller* cameraController);
		~ShadowMapMasterRenderer();

		void Render(const glm::vec3& lightDirection);

		inline const glm::mat4& GetLS() const { return projectionView; }
		inline  unsigned int GetShadowMap() { return frameBuffer->GetShadowMap(); }
	private:
		void Prepare(const glm::vec3& lightDirection);
		void Finish();

		void UpdateOrthoProjection();
		void UpdateLightView(const glm::vec3& lightDirection);

	private:

		Perspective_Camera_Controller* cameraController;

		ShadowMapEntityRenderer* entityRenderer;
		ShadowFrameBuffer* frameBuffer;
		ShadowBox* shadowBox;

		Ref<Shader> shader;

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 lightView = glm::mat4(1.0f);
		glm::mat4 projectionView = glm::mat4(1.0f);
		glm::mat4 offSet;
	};
}