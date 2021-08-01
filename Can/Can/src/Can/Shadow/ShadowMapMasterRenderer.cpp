#include "canpch.h"
#include "ShadowMapMasterRenderer.h"
#include <glad/glad.h>

namespace Can
{
	ShadowMapMasterRenderer::ShadowMapMasterRenderer(Perspective_Camera_Controller* cameraController)
		: cameraController(cameraController)
	{
		shader = Shader::Create("assets/shaders/simpleDepth.glsl");
		shadowBox = new ShadowBox(lightView, cameraController);
		frameBuffer = new ShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		entityRenderer = new ShadowMapEntityRenderer(shader, projectionView);

	}
	ShadowMapMasterRenderer::~ShadowMapMasterRenderer()
	{
		delete entityRenderer;
		delete frameBuffer;
		delete shadowBox;
	}

	void ShadowMapMasterRenderer::Render(const glm::vec3& lightDirection)
	{
		shadowBox->Update();
		Prepare(lightDirection);
		entityRenderer->Render();
		Finish();
	}
	void ShadowMapMasterRenderer::Prepare(const glm::vec3& lightDirection)
	{
		UpdateOrthoProjection();
		UpdateLightView(lightDirection);

		frameBuffer->Bind();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		projectionView = projection * lightView;
		shader->SetMat4("ligthSpaceMatrix", projectionView);
	}

	void ShadowMapMasterRenderer::Finish()
	{
		shader->Unbind();
		frameBuffer->Unbind();
	}

	void ShadowMapMasterRenderer::UpdateOrthoProjection()
	{
		float halfwidth = shadowBox->GetWidth() / 2.0f;
		float halfheight = shadowBox->GetHeight() / 2.0f;
		float halfLength = shadowBox->GetLength() / 2.0f;
		projection = glm::ortho(
			-halfwidth, 
			+halfwidth, 
			-halfheight,
			+halfheight, 
			-halfLength, 
			+halfLength
		);
	}
	void ShadowMapMasterRenderer::UpdateLightView(const glm::vec3& lightDirection)
	{
		lightView = glm::lookAt(shadowBox->GetCenter(), shadowBox->GetCenter() + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}