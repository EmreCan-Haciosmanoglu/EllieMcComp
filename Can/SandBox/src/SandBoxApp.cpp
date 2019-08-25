#include "canpch.h"
#include "Can.h"

class ExampleLayer : public Can::Layer::Layer
{
public:
	ExampleLayer()
		: Can::Layer::Layer("Example")
		, m_Camera(-16.0f, 16.0f, -9.0f, 9.0f)
		, m_CameraPosition(0.0f)
		, m_CameraRotation(0.0f)
		, m_CameraMoveSpeed(0.1f)
		, m_CameraRotateSpeed(2.0f)
	{
		m_VertexArray.reset(Can::VertexArray::Create());

		float vertices[3 * (3 + 4)] = {
			-0.57f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.57f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		std::shared_ptr<Can::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Can::VertexBuffer::Create(vertices, sizeof(vertices)));

		Can::BufferLayout layout = {
			{Can::ShaderDataType::Float3, "a_Position"},
			{Can::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3]{ 0,1,2 };
		std::shared_ptr<Can::IndexBuffer> indexBuffer;
		indexBuffer.reset(Can::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string veS = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;

			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
			}
		)";

		std::string frS = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(new Can::Shader(veS, frS));
	}

	void OnUpdate() override
	{
		if (Can::Input::IsKeyPressed(CAN_KEY_LEFT))
		{
			m_CameraPosition -= glm::vec3(m_CameraMoveSpeed, 0.0f, 0.0f);
		}
		else if (Can::Input::IsKeyPressed(CAN_KEY_RIGHT))
		{
			m_CameraPosition += glm::vec3(m_CameraMoveSpeed, 0.0f, 0.0f);
		}

		if (Can::Input::IsKeyPressed(CAN_KEY_UP))
		{
			m_CameraPosition += glm::vec3(0.0f, m_CameraMoveSpeed, 0.0f);
		}
		else if (Can::Input::IsKeyPressed(CAN_KEY_DOWN))
		{
			m_CameraPosition -= glm::vec3(0.0f, m_CameraMoveSpeed, 0.0f);
		}

		if (Can::Input::IsKeyPressed(CAN_KEY_A))
		{
			m_CameraRotation += m_CameraRotateSpeed;
		}
		else if (Can::Input::IsKeyPressed(CAN_KEY_D))
		{
			m_CameraRotation -= m_CameraRotateSpeed;
		}
		
		Can::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
		Can::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Can::Renderer::BeginScene(m_Camera);
		
		Can::Renderer::Submit(m_Shader, m_VertexArray);
		
		Can::Renderer::EndScene();
	}

	void OnEvent(Can::Event::Event& event) override
	{
	}

private:
	std::shared_ptr<Can::Shader> m_Shader;
	std::shared_ptr <Can::VertexArray> m_VertexArray;

	Can::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed;

	float m_CameraRotation;
	float m_CameraRotateSpeed;

};

class Sandbox : public Can::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Can::Application* Can::CreateApplication()
{
	return new Sandbox();
}