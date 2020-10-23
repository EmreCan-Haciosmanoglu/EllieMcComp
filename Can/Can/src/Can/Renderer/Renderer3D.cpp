#include "canpch.h"
#include "Renderer3D.h"
#include "DeleteLaterRenderer2D.h"
#include "Can.h"

#include <glad/glad.h>


namespace Can
{
	static std::vector<Object*> s_Objects;

	void Renderer3D::Init()
	{
		CAN_PROFILE_FUNCTION();

	}
	void Renderer3D::Shutdown()
	{
		CAN_PROFILE_FUNCTION();

	}

	void Renderer3D::BeginScene(const Camera::PerspectiveCamera& camera)
	{
		CAN_PROFILE_FUNCTION();

		for (Object* o : s_Objects)
		{
			if (!o->enabled)
				continue;
			Prefab* prefab = o->prefab;
			prefab->shader->Bind();
			prefab->shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		}
	}
	void Renderer3D::EndScene()
	{
		CAN_PROFILE_FUNCTION();

	}

	void Renderer3D::AddObject(Object* obj)
	{
		CAN_PROFILE_FUNCTION();

		s_Objects.push_back(obj);
	}

	void Renderer3D::DeleteObject(Object* obj)
	{
		CAN_PROFILE_FUNCTION();

		auto e = std::find(s_Objects.begin(), s_Objects.end(), obj);

		if (e != s_Objects.end()) {
			s_Objects.erase(e);
		}
	}
	OutputTest Renderer3D::Test(const Camera::OrthographicCamera& camera)
	{
		FramebufferSpecification spec;
		spec.Width = 1024;
		spec.Height = 1024;


		// configure depth map FBO
		// ---------------------
		const unsigned int SHADOW_WIDTH = 1024;
		const unsigned int SHADOW_HEIGHT = 1024;
		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);

		// create depth texture
		unsigned int depthMap;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//lighting info
		glm::vec3 lightPos{ 3.0f, 5.0f, 5.0f };


		// render depth of the scene to texture (from light's perspective)
		static Ref<Shader> simpleDepthShader = Shader::Create("assets/shaders/simpleDepth.glsl");
		simpleDepthShader->Bind();
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 100.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3{ 3.0f, 0.0f, -15.0f }, glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;

		simpleDepthShader->SetMat4("ligthSpaceMatrix", lightSpaceMatrix);

		RenderCommand::SetViewport(0, 0, spec.Width, spec.Height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		RenderCommand::Clear();
		DrawObjectsForShadowMap(simpleDepthShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Window& window = Application::Get().GetWindow();
		RenderCommand::SetViewport(0, 0, window.GetWidth(), window.GetHeight());

		return { lightSpaceMatrix, depthMap };
	}

	void Renderer3D::DrawObjects(const OutputTest& outputTest, const Camera::PerspectiveCamera& camera)
	{
		CAN_PROFILE_FUNCTION();

		//light
		static glm::vec3 lightRay{ 1.0f, 0.0f, 0.3f };
		lightRay = glm::rotate(lightRay, glm::radians(0.05f), glm::vec3{ 0.0f, 0.0f, 1.0f });


		for (Object* obj : s_Objects)
		{
			if (!obj->enabled)
				continue;
			Prefab* prefab = obj->prefab;

			prefab->shader->Bind();
			glActiveTexture(GL_TEXTURE16);
			glBindTexture(GL_TEXTURE_2D, outputTest.depthMap);
			prefab->shader->SetMat4("u_LightSpace", outputTest.lightSpaceMatrix);
			prefab->shader->SetMat4("u_Transform", obj->transform);
			prefab->shader->SetFloat4("u_TintColor", obj->tintColor);

			glm::vec3 rotatedLightRay = glm::rotate(lightRay, -(obj->rotation.y), glm::vec3{ 0.0f, 1.0f , 0.0f });
			prefab->shader->SetFloat3("u_LightPos", rotatedLightRay);
			prefab->shader->SetFloat3("u_ViewPos", camera.GetPosition());
			prefab->vertexArray->Bind();
			for (size_t i = 0; i < prefab->textureCount; i++)
				prefab->textures[i]->Bind((uint32_t)i);
			if (prefab->textures[0])
				for (size_t i = prefab->textureCount; i < MAX_TEXTURE_SLOTS; i++)
					prefab->textures[0]->Bind((uint32_t)i);

			RenderCommand::DrawIndexed(prefab->vertexArray);
		}
	}

	void Renderer3D::DrawObjectsForShadowMap(const Ref<Shader> shadowShader)
	{
		CAN_PROFILE_FUNCTION();
		for (Object* obj : s_Objects)
		{
			if (!obj->enabled)
				continue;

			shadowShader->SetMat4("model", obj->transform);
			obj->prefab->vertexArray->Bind();
			RenderCommand::DrawIndexed(obj->prefab->vertexArray);
		}
	}
}