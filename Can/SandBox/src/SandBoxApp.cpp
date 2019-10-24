#include "canpch.h"
#include "Can.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Can::Layer::Layer
{
public:
	ExampleLayer()
		: Can::Layer::Layer("Example")
		, m_Camera(-16.0f, 16.0f, -9.0f, 9.0f, -10.0f, 10.0f)
		, m_CameraPosition(0.0f)
		, m_CameraRotation(0.0f)
		, m_CameraMoveSpeed(10.0f)
		, m_CubePosition(0.0f)
		, m_CameraRotateSpeed(90.0f)
	{
		m_SquareVertexArray.reset(Can::VertexArray::Create());

		float vertices[4 * (3 + 2)] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Can::Ref<Can::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Can::VertexBuffer::Create(vertices, sizeof(vertices)));

		Can::BufferLayout layout = {
			{Can::ShaderDataType::Float3, "a_Position"},
			{Can::ShaderDataType::Float2, "a_TexCoord"}
		};

		squareVertexBuffer->SetLayout(layout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t indices[6]{ 0,1,2,0,2,3 };
		Can::Ref<Can::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Can::IndexBuffer::Create(indices, 6));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);


		std::string veS = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
			}
		)";

		std::string frS = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			uniform vec3 u_Color;

			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
				//color = vec4(u_Color,1.0);
			}
		)";

		m_SquareShader.reset(Can::Shader::Create("assets/shaders/Texture.glsl"));
		m_SquareTexture = Can::Texture2D::Create("assets/textures/Man.png");
		m_NameTexture = Can::Texture2D::Create("assets/textures/Name.png");

		m_CubeShader.reset(Can::Shader::Create(veS, frS));


		m_CubeVertexArray.reset(Can::VertexArray::Create());

		float cubeVertexPositions[8 * (3 + 4) * 3] = {
			+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Red		-- 0	-0
			+1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Blue	-- 1	-0
			+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Magenta	-- 2	-0
														    		  		
			+1.0f, +1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Red		-- 3	-1
			+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Blue	-- 4	-1
			+1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Cyan	-- 5	-1
														    		  		
			+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Red		-- 6	-2
			+1.0f, -1.0f, +1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Yellow	-- 7	-2
			+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Magenta	-- 8	-2
														    		  		
			+1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Red		-- 9	-3
			+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Yellow	-- 10	-3
			+1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Cyan	-- 11	-3
														    		  		
			-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Blue	-- 12	-4
			-1.0f, +1.0f, +1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Green	-- 13	-4
			-1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Magenta	-- 14	-4
														    		  		
			-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Blue	-- 15	-5
			-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Green	-- 16	-5
			-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Cyan	-- 17	-5
														    		  		
			-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Green	-- 18	-6
			-1.0f, -1.0f, +1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Yellow	-- 19	-6
			-1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Magenta	-- 20	-6
														    		  		
			-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Green	-- 21	-7
			-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Yellow	-- 22	-7
			-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // Cyan	-- 23	-7
		};
		Can::Ref<Can::VertexBuffer> vertexPositionBuffer;
		vertexPositionBuffer.reset(Can::VertexBuffer::Create(cubeVertexPositions, sizeof(cubeVertexPositions)));

		Can::BufferLayout Positionlayout = {
			{Can::ShaderDataType::Float3, "a_Position"},
			{Can::ShaderDataType::Float4, "a_Color"}
		};

		vertexPositionBuffer->SetLayout(Positionlayout);
		m_CubeVertexArray->AddVertexBuffer(vertexPositionBuffer);

		uint32_t cubeIndices[12 * 3] = {
			0,  6,  9,  // Red
			0,  9,  3,  // Red
			4,  15, 12, // Blue
			4,  12, 1,  // Blue
			16, 21, 18, // Green
			16, 18, 13, // Green
			7,  19, 22, // Yellow
			7,  22, 10, // Yellow
			5,  11, 17, // Cyan
			17, 11, 23, // Cyan
			14, 20, 8,  // Magenta
			14, 8,  2   // Magenta
		};

		Can::Ref<Can::IndexBuffer> cIndexBuffer;
		cIndexBuffer.reset(Can::IndexBuffer::Create(cubeIndices, 12 * 3));
		m_CubeVertexArray->SetIndexBuffer(cIndexBuffer);

		Can::Ref<Can::OpenGLShader> openglshader = std::dynamic_pointer_cast<Can::OpenGLShader>(m_SquareShader);
		if (openglshader)
		{
			openglshader->Bind();
			openglshader->UploadUniformInt("u_Texture", 0);
		}

	}

	void OnUpdate(Can::TimeStep ts) override
	{
		float time = ts;
		//CAN_CORE_INFO("Timestep: {0}s ({1}ms)", time ,ts.GetMiliseconds());
		
		//Camera Movement
		{
			if (Can::Input::IsKeyPressed(CAN_KEY_LEFT))
			{
				m_CameraPosition -= glm::vec3(m_CameraMoveSpeed * time, 0.0f, 0.0f);
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_RIGHT))
			{
				m_CameraPosition += glm::vec3(m_CameraMoveSpeed * time, 0.0f, 0.0f);
			}

			if (Can::Input::IsKeyPressed(CAN_KEY_UP))
			{
				m_CameraPosition += glm::vec3(0.0f, m_CameraMoveSpeed * time, 0.0f);
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_DOWN))
			{
				m_CameraPosition -= glm::vec3(0.0f, m_CameraMoveSpeed * time, 0.0f);
			}
		}

		//Camera Rotation
		{
			if (Can::Input::IsKeyPressed(CAN_KEY_A))
			{
				m_CameraRotation.z += m_CameraRotateSpeed * time;
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_D))
			{
				m_CameraRotation.z -= m_CameraRotateSpeed * time;
			}

			if (Can::Input::IsKeyPressed(CAN_KEY_W))
			{
				m_CameraRotation.y += m_CameraRotateSpeed * time;
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_S))
			{
				m_CameraRotation.y -= m_CameraRotateSpeed * time;
			}

			if (Can::Input::IsKeyPressed(CAN_KEY_Q))
			{
				m_CameraRotation.x += m_CameraRotateSpeed * time;
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_E))
			{
				m_CameraRotation.x -= m_CameraRotateSpeed * time;
			}
		}

		//Cube Movement
		{
			if (Can::Input::IsKeyPressed(CAN_KEY_G))
			{
				m_CubePosition.x -= m_CameraMoveSpeed * time;
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_J))
			{
				m_CubePosition.x += m_CameraMoveSpeed * time;
			}

			if (Can::Input::IsKeyPressed(CAN_KEY_Y))
			{
				m_CubePosition.y += m_CameraMoveSpeed * time;
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_H))
			{
				m_CubePosition.y -= m_CameraMoveSpeed * time;
			}

			if (Can::Input::IsKeyPressed(CAN_KEY_T))
			{
				m_CubePosition.z += m_CameraMoveSpeed * time;
			}
			else if (Can::Input::IsKeyPressed(CAN_KEY_U))
			{
				m_CubePosition.z -= m_CameraMoveSpeed * time;
			}
		}

		Can::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
		Can::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Can::Renderer::BeginScene(m_Camera);

		m_SquareTexture->Bind();
		Can::Renderer::Submit(m_SquareShader, m_SquareVertexArray);

		m_NameTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CubePosition);
		Can::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
		
		Can::Ref<Can::OpenGLShader> openglshader = std::dynamic_pointer_cast<Can::OpenGLShader>(m_CubeShader);
		if (openglshader)
		{
			openglshader->Bind();
			openglshader->UploadUniformFloat3("u_Color", m_LeftColor);
		}
		transform = glm::translate(transform, m_CubePosition);
		Can::Renderer::Submit(m_CubeShader, m_CubeVertexArray, transform);

		Can::Renderer::EndScene();
	}

	void OnEvent(Can::Event::Event& event) override
	{
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Left Color", glm::value_ptr(m_LeftColor));
		ImGui::ColorEdit3("Right Color", glm::value_ptr(m_RightColor));
		ImGui::ColorEdit3("Top Color", glm::value_ptr(m_TopColor));
		ImGui::ColorEdit3("Bottom Color", glm::value_ptr(m_BottomColor));
		ImGui::ColorEdit3("Back Color", glm::value_ptr(m_BackColor));
		ImGui::ColorEdit3("Front Color", glm::value_ptr(m_FrontColor));
		ImGui::End();
	}

private:
	Can::Ref<Can::Shader> m_CubeShader;
	Can::Ref<Can::Shader> m_SquareShader;

	Can::Ref<Can::VertexArray> m_CubeVertexArray;
	Can::Ref<Can::VertexArray> m_SquareVertexArray;

	Can::Ref<Can::Texture2D> m_SquareTexture;
	Can::Ref<Can::Texture2D> m_NameTexture;

	Can::Camera::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed;

	glm::vec3 m_CameraRotation;
	float m_CameraRotateSpeed;

	glm::vec3 m_CubePosition;

	glm::vec3 m_LeftColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_RightColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_TopColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_BottomColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_BackColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_FrontColor = glm::vec3(0.0f, 0.0f, 0.0f);

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