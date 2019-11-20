#include "canpch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Can
{
	struct Renderer3DStorage
	{
		Ref<VertexArray> CubeVertexArray;
		Ref<Shader> CubeShader;
	};
	static Renderer3DStorage* s_3DData;
	
	void Renderer3D::Init()
	{
		s_3DData = new Renderer3DStorage();
		s_3DData->CubeVertexArray = VertexArray::Create();

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

		Ref< VertexBuffer> cubeVB;
		cubeVB.reset(VertexBuffer::Create(cubeVertexPositions, sizeof(cubeVertexPositions)));
		cubeVB->SetLayout({
		   { ShaderDataType::Float3, "a_Position"},
		   { ShaderDataType::Float4, "a_Color"}
			});
		s_3DData->CubeVertexArray->AddVertexBuffer(cubeVB);

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

		Ref<IndexBuffer> cubeIB;
		cubeIB.reset(IndexBuffer::Create(cubeIndices, 12 * 3));
		s_3DData->CubeVertexArray->SetIndexBuffer(cubeIB);

		s_3DData->CubeShader = Shader::Create("assets/shaders/Cube.glsl");

		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetInt("u_Texture", 0);
	}
	void Renderer3D::Shutdown()
	{
		delete s_3DData;
	}

	void Renderer3D::BeginScene(const Camera::PerspectiveCamera& camera)
	{
		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer3D::EndScene()
	{
	}
	
	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& scale)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);

		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetMat4("u_Transform", transform);

		s_3DData->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_3DData->CubeVertexArray);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& scale, const Ref<Texture2D>& texture)
	{
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);

		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetMat4("u_Transform", transform);

		s_3DData->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_3DData->CubeVertexArray);
	}
}