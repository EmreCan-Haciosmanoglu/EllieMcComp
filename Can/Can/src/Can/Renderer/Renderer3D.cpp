#include "canpch.h"
#include "Renderer3D.h"

#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

#define RESOLUTION 20

#define SCALE 2
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
		glm::vec3 localUps[6] = {
			{+1.0f, +0.0f, +0.0f},
			{-1.0f, +0.0f, +0.0f},
			{+0.0f, +1.0f, +0.0f},
			{+0.0f, -1.0f, +0.0f},
			{+0.0f, +0.0f, +1.0f},
			{+0.0f, +0.0f, -1.0f}
		};

		float m_Vertices[6 * (RESOLUTION - 1) * (RESOLUTION - 1) * 2 * 3 * (3 + 4 + 3)];
		uint32_t m_Indices[6 * (RESOLUTION - 1) * (RESOLUTION - 1) * 2 * 3];

		int vertexIndex = 0;
		for (int k = 0; k < 6; k++)
		{
			glm::vec3 localUp = localUps[k];
			glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
			glm::vec3 axisB = glm::cross(localUp, axisA);
			for (float j = 0; j < RESOLUTION - 1; j++)
			{
				for (float i = 0; i < RESOLUTION - 1; i++)
				{
					{
						float x = i;
						float y = j;
						glm::vec2 percent = glm::vec2(x / (RESOLUTION - 1.0f), y / (RESOLUTION - 1.0f));
						glm::vec3 pointOnUnitCube = glm::vec3(
							localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
							localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
							localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
						);
						glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

						m_Vertices[vertexIndex++] = pointOnUnitSphere.x * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.y * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.z * SCALE;

						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.y;
						m_Vertices[vertexIndex++] = 1.0f;
						vertexIndex += 3;
					}
					{
						float x = i + 1;
						float y = j;
						glm::vec2 percent = glm::vec2(x / (RESOLUTION - 1.0f), y / (RESOLUTION - 1.0f));
						glm::vec3 pointOnUnitCube = glm::vec3(
							localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
							localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
							localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
						);
						glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

						m_Vertices[vertexIndex++] = pointOnUnitSphere.x * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.y * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.z * SCALE;

						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.y;
						m_Vertices[vertexIndex++] = 1.0f;
						vertexIndex += 3;
					}
					{
						float x = i + 1;
						float y = j + 1;
						glm::vec2 percent = glm::vec2(x / (RESOLUTION - 1.0f), y / (RESOLUTION - 1.0f));
						glm::vec3 pointOnUnitCube = glm::vec3(
							localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
							localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
							localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
						);
						glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

						m_Vertices[vertexIndex++] = pointOnUnitSphere.x * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.y * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.z * SCALE;

						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.y;
						m_Vertices[vertexIndex++] = 1.0f;
						vertexIndex += 3;
					}
					{
						float x = i;
						float y = j;
						glm::vec2 percent = glm::vec2(x / (RESOLUTION - 1.0f), y / (RESOLUTION - 1.0f));
						glm::vec3 pointOnUnitCube = glm::vec3(
							localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
							localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
							localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
						);
						glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

						m_Vertices[vertexIndex++] = pointOnUnitSphere.x * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.y * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.z * SCALE;

						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.y;
						m_Vertices[vertexIndex++] = 1.0f;
						vertexIndex += 3;
					}
					{
						float x = i + 1;
						float y = j + 1;
						glm::vec2 percent = glm::vec2(x / (RESOLUTION - 1.0f), y / (RESOLUTION - 1.0f));
						glm::vec3 pointOnUnitCube = glm::vec3(
							localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
							localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
							localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
						);
						glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

						m_Vertices[vertexIndex++] = pointOnUnitSphere.x * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.y * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.z * SCALE;

						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.y;
						m_Vertices[vertexIndex++] = 1.0f;
						vertexIndex += 3;
					}
					{
						float x = i;
						float y = j + 1;
						glm::vec2 percent = glm::vec2(x / (RESOLUTION - 1.0f), y / (RESOLUTION - 1.0f));
						glm::vec3 pointOnUnitCube = glm::vec3(
							localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
							localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
							localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
						);
						glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

						m_Vertices[vertexIndex++] = pointOnUnitSphere.x * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.y * SCALE;
						m_Vertices[vertexIndex++] = pointOnUnitSphere.z * SCALE;

						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.x;
						m_Vertices[vertexIndex++] = 1.0f * percent.y;
						m_Vertices[vertexIndex++] = 1.0f;
						vertexIndex += 3;
					}
				}
			}
		}
		vertexIndex = 0;
		for (int k = 0; k < 6; k++)
		{
			for (float j = 0; j < RESOLUTION - 1; j++)
			{
				for (float i = 0; i < RESOLUTION - 1; i++)
				{
					glm::vec3 a00(m_Vertices[vertexIndex + 0 + 0],  m_Vertices[vertexIndex + 0 + 1],  m_Vertices[vertexIndex + 0 + 2]);
					glm::vec3 a10(m_Vertices[vertexIndex + 10 + 0], m_Vertices[vertexIndex + 10 + 1], m_Vertices[vertexIndex + 10 + 2]);
					glm::vec3 a11(m_Vertices[vertexIndex + 20 + 0], m_Vertices[vertexIndex + 20 + 1], m_Vertices[vertexIndex + 20 + 2]);
					glm::vec3 a01(m_Vertices[vertexIndex + 50 + 0], m_Vertices[vertexIndex + 50 + 1], m_Vertices[vertexIndex + 50 + 2]);

					glm::vec3 u1 = a11 - a00;
					glm::vec3 v1 = a10 - a00;

					glm::vec3 u2 = a01 - a00;
					glm::vec3 v2 = a11 - a00;

					glm::vec3 norm1 = glm::cross(v1, u1);
					glm::vec3 norm2 = glm::cross(v2, u2);

					m_Vertices[vertexIndex + 0 + 7] = norm1.x;
					m_Vertices[vertexIndex + 0 + 8] = norm1.y;
					m_Vertices[vertexIndex + 0 + 9] = norm1.z;

					m_Vertices[vertexIndex + 10 + 7] = norm1.x;
					m_Vertices[vertexIndex + 10 + 8] = norm1.y;
					m_Vertices[vertexIndex + 10 + 9] = norm1.z;

					m_Vertices[vertexIndex + 20 + 7] = norm1.x;
					m_Vertices[vertexIndex + 20 + 8] = norm1.y;
					m_Vertices[vertexIndex + 20 + 9] = norm1.z;

					m_Vertices[vertexIndex + 30 + 7] = norm2.x;
					m_Vertices[vertexIndex + 30 + 8] = norm2.y;
					m_Vertices[vertexIndex + 30 + 9] = norm2.z;

					m_Vertices[vertexIndex + 40 + 7] = norm2.x;
					m_Vertices[vertexIndex + 40 + 8] = norm2.y;
					m_Vertices[vertexIndex + 40 + 9] = norm2.z;

					m_Vertices[vertexIndex + 50 + 7] = norm2.x;
					m_Vertices[vertexIndex + 50 + 8] = norm2.y;
					m_Vertices[vertexIndex + 50 + 9] = norm2.z;
					vertexIndex += 60;
				}
			}
		}

		int indicesIndex = 0;
		vertexIndex = 0;
		for (int k = 0; k < 6; k++)
		{
			for (int j = 0; j < RESOLUTION - 1; j++)
			{
				for (int i = 0; i < RESOLUTION - 1; i++)
				{
					m_Indices[indicesIndex++] = vertexIndex++;
					m_Indices[indicesIndex++] = vertexIndex++;
					m_Indices[indicesIndex++] = vertexIndex++;
					m_Indices[indicesIndex++] = vertexIndex++;
					m_Indices[indicesIndex++] = vertexIndex++;
					m_Indices[indicesIndex++] = vertexIndex++;
				}
			}
		}

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
		cubeVB.reset(VertexBuffer::Create(m_Vertices, sizeof(m_Vertices)));
		cubeVB->SetLayout({
		   { ShaderDataType::Float3, "a_Position"},
		   { ShaderDataType::Float4, "a_Color"},
		   { ShaderDataType::Float3, "a_Normal"}
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
		cubeIB.reset(IndexBuffer::Create(m_Indices, 6 * (RESOLUTION - 1) * (RESOLUTION - 1) * 2 * 3));
		s_3DData->CubeVertexArray->SetIndexBuffer(cubeIB);
		
		s_3DData->CubeShader = Shader::Create("assets/shaders/Cube.glsl");

		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetInt("u_Texture", 0);
		s_3DData->CubeShader->SetFloat3("u_LightPos", { 4.0f, 0.0f, 0.0f });
	}
	void Renderer3D::Shutdown()
	{
		delete s_3DData;
	}

	void Renderer3D::BeginScene(const Camera::PerspectiveCamera & camera)
	{
		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::DrawCube(const glm::vec3 & position, const glm::vec3 & scale)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);

		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetMat4("u_Transform", transform);

		s_3DData->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_3DData->CubeVertexArray);
	}
	void Renderer3D::DrawCube(const glm::vec3 & position, const glm::vec3 & scale, const Ref<Texture2D> & texture)
	{
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);

		s_3DData->CubeShader->Bind();
		s_3DData->CubeShader->SetMat4("u_Transform", transform);

		s_3DData->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_3DData->CubeVertexArray);
	}
}