#include "canpch.h"
#include "Can.h"

class ExampleLayer : public Can::Layer::Layer
{
public:
	ExampleLayer()
		: Can::Layer::Layer("Example")
		, m_Camera(-16.0f, 16.0f, -9.0f, 9.0f, -10.0f, 10.0f)
		, m_CameraPosition(0.0f)
		, m_CameraRotation(0.0f)
		, m_CameraMoveSpeed(10.0f)
		, m_CameraRotateSpeed(90.0f)
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


		VertexArray.reset(Can::VertexArray::Create());

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
		std::shared_ptr<Can::VertexBuffer> vertexPositionBuffer;
		vertexPositionBuffer.reset(Can::VertexBuffer::Create(cubeVertexPositions, sizeof(cubeVertexPositions)));

		Can::BufferLayout Positionlayout = {
			{Can::ShaderDataType::Float3, "a_Position"},
			{Can::ShaderDataType::Float4, "a_Color"}
		};

		vertexPositionBuffer->SetLayout(Positionlayout);
		VertexArray->AddVertexBuffer(vertexPositionBuffer);

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

		std::shared_ptr<Can::IndexBuffer> cIndexBuffer;
		cIndexBuffer.reset(Can::IndexBuffer::Create(cubeIndices, 12 * 3));
		VertexArray->SetIndexBuffer(cIndexBuffer);
	}

	void OnUpdate(Can::TimeStep ts) override
	{
		float time = ts;
		//CAN_CORE_INFO("Timestep: {0}s ({1}ms)", time ,ts.GetMiliseconds());
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

		Can::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
		Can::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Can::Renderer::BeginScene(m_Camera);

		Can::Renderer::Submit(m_Shader, m_VertexArray);
		Can::Renderer::Submit(m_Shader, VertexArray);

		Can::Renderer::EndScene();
	}

	void OnEvent(Can::Event::Event& event) override
	{
	}

private:
	std::shared_ptr<Can::Shader> m_Shader;
	std::shared_ptr <Can::VertexArray> m_VertexArray;

	std::shared_ptr <Can::VertexArray> VertexArray;

	Can::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed;

	glm::vec3 m_CameraRotation;
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