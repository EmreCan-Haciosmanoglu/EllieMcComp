#include "canpch.h"
#include "SandBox2DLayer.h"
#include <glm/gtc/type_ptr.hpp>

SandBox2DLayer::SandBox2DLayer()
	:Layer("2DLayer")
	, m_CameraController(1280.0f / 720.0f, true)
{
}

void SandBox2DLayer::OnAttach()
{
	m_SquareVA = Can::VertexArray::Create();

	float vertices[4 * (3)] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	Can::Ref<Can::VertexBuffer> squareVB;
	squareVB.reset(Can::VertexBuffer::Create(vertices, sizeof(vertices)));
	squareVB->SetLayout({
		{Can::ShaderDataType::Float3, "a_Position"}
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t indices[6]{ 0,1,2,2,3,0 };
	Can::Ref<Can::IndexBuffer> squareIB;
	squareIB.reset(Can::IndexBuffer::Create(indices, 6));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_SquareShader = Can::Shader::Create("assets/shaders/Square.glsl");
}

void SandBox2DLayer::OnDetach()
{
}

void SandBox2DLayer::OnUpdate(Can::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Can::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer::BeginScene(m_CameraController.GetCamera());


	Can::Ref<Can::OpenGLShader> openglshader = std::dynamic_pointer_cast<Can::OpenGLShader>(m_SquareShader);
	if (openglshader)
	{
		openglshader->Bind();
		openglshader->UploadUniformFloat4("u_Color", m_SquareColor);
	}

	Can::Renderer::Submit(m_SquareShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Can::Renderer::EndScene();
}

void SandBox2DLayer::OnEvent(Can::Event::Event& event)
{
	m_CameraController.OnEvent(event);
}

void SandBox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
