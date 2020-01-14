#include "canpch.h"
#include "SandBox3DApp.h"
#include "Can/EntryPoint.h"


Can::Application* Can::CreateApplication()
{
	return new Sandbox3D();
}

Sandbox3D::Sandbox3D()
	: m_ShapeGenerator(new ShapeGenerator(new ShapeSettings(2)))
	, m_Scene(new Scene(this))
	, m_Debug(new Debug(this))
	, m_SphereFaces(new Can::Object* [6 * m_FaceResolution * m_FaceResolution])
{
	int index = 0;
	for (int k = 0; k < 6; k++)
	{
		for (int i = 0; i < m_FaceResolution; i++)
		{
			for (int j = 0; j < m_FaceResolution; j++)
			{
				m_SphereFaces[index] = new Can::Object();
				ConstructSphereFace(
					m_SphereFaces[index],
					(1.0f * i) / m_FaceResolution,
					(1.0f * j) / m_FaceResolution,
					m_LocalUps[k]);
				Can::Renderer3D::AddObject(m_SphereFaces[index]);
				index++;
			}
		}
	}
	PushLayer(m_Scene);
	PushOverlay(m_Debug);

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	bool res = loadOBJ("assets/objects/test.obj", vertices, uvs, normals);
	if (res)
	{
		m_TestObject = ConstructObject("assets/shaders/Object.glsl", "assets/objects/Test.png", vertices, uvs, normals);
		//Can::Renderer3D::AddObject(m_TestObject);
	}
}

Sandbox3D::~Sandbox3D()
{
}

float* Sandbox3D::CreateSphere(const glm::vec3& localUp, float xOffSet, float yOffSet)
{
	float* result = new float[m_Resolution * m_Resolution * 2 * 3 * (3 + 4 + 3)];

	int vertexIndex = 0;
	glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
	glm::vec3 axisB = glm::cross(localUp, axisA);
	for (float j = 0; j < m_Resolution; j++)
	{
		for (float i = 0; i < m_Resolution; i++)
		{
			{
				float x = i;
				float y = j;
				glm::vec2 percent = glm::vec2(
					(x / (m_Resolution * m_FaceResolution)) + xOffSet,
					(y / (m_Resolution * m_FaceResolution)) + yOffSet
				);
				glm::vec3 pointOnUnitCube = glm::vec3(
					localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
					localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
					localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
				);
				result[vertexIndex++] = pointOnUnitCube.x;
				result[vertexIndex++] = pointOnUnitCube.y;
				result[vertexIndex++] = pointOnUnitCube.z;

				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.y;
				result[vertexIndex++] = 1.0f;
				vertexIndex += 3;
			}
			{
				float x = i + 1;
				float y = j;
				glm::vec2 percent = glm::vec2(
					(x / (m_Resolution * m_FaceResolution)) + xOffSet,
					(y / (m_Resolution * m_FaceResolution)) + yOffSet
				);
				glm::vec3 pointOnUnitCube = glm::vec3(
					localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
					localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
					localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
				);
				result[vertexIndex++] = pointOnUnitCube.x;
				result[vertexIndex++] = pointOnUnitCube.y;
				result[vertexIndex++] = pointOnUnitCube.z;

				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.y;
				result[vertexIndex++] = 1.0f;
				vertexIndex += 3;
			}
			{
				float x = i + 1;
				float y = j + 1;
				glm::vec2 percent = glm::vec2(
					(x / (m_Resolution * m_FaceResolution)) + xOffSet,
					(y / (m_Resolution * m_FaceResolution)) + yOffSet
				);
				glm::vec3 pointOnUnitCube = glm::vec3(
					localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
					localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
					localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
				);
				result[vertexIndex++] = pointOnUnitCube.x;
				result[vertexIndex++] = pointOnUnitCube.y;
				result[vertexIndex++] = pointOnUnitCube.z;

				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.y;
				result[vertexIndex++] = 1.0f;
				vertexIndex += 3;
			}
			{
				float x = i;
				float y = j;
				glm::vec2 percent = glm::vec2(
					(x / (m_Resolution * m_FaceResolution)) + xOffSet,
					(y / (m_Resolution * m_FaceResolution)) + yOffSet
				);
				glm::vec3 pointOnUnitCube = glm::vec3(
					localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
					localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
					localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
				);
				result[vertexIndex++] = pointOnUnitCube.x;
				result[vertexIndex++] = pointOnUnitCube.y;
				result[vertexIndex++] = pointOnUnitCube.z;

				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.y;
				result[vertexIndex++] = 1.0f;
				vertexIndex += 3;
			}
			{
				float x = i + 1;
				float y = j + 1;
				glm::vec2 percent = glm::vec2(
					(x / (m_Resolution * m_FaceResolution)) + xOffSet,
					(y / (m_Resolution * m_FaceResolution)) + yOffSet
				);
				glm::vec3 pointOnUnitCube = glm::vec3(
					localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
					localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
					localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
				);
				result[vertexIndex++] = pointOnUnitCube.x;
				result[vertexIndex++] = pointOnUnitCube.y;
				result[vertexIndex++] = pointOnUnitCube.z;

				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.y;
				result[vertexIndex++] = 1.0f;
				vertexIndex += 3;
			}
			{
				float x = i;
				float y = j + 1;
				glm::vec2 percent = glm::vec2(
					(x / (m_Resolution * m_FaceResolution)) + xOffSet,
					(y / (m_Resolution * m_FaceResolution)) + yOffSet
				);
				glm::vec3 pointOnUnitCube = glm::vec3(
					localUp.x + (percent.x - 0.5f) * 2.0f * axisA.x + (percent.y - 0.5f) * 2.0f * axisB.x,
					localUp.y + (percent.x - 0.5f) * 2.0f * axisA.y + (percent.y - 0.5f) * 2.0f * axisB.y,
					localUp.z + (percent.x - 0.5f) * 2.0f * axisA.z + (percent.y - 0.5f) * 2.0f * axisB.z
				);
				result[vertexIndex++] = pointOnUnitCube.x;
				result[vertexIndex++] = pointOnUnitCube.y;
				result[vertexIndex++] = pointOnUnitCube.z;

				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.x;
				result[vertexIndex++] = 1.0f * percent.y;
				result[vertexIndex++] = 1.0f;
				vertexIndex += 3;
			}
		}
	}

	return result;
}

float* Sandbox3D::ShapeSphere(float* cube)
{
	int vertexIndex = 0;
	for (float j = 0; j < m_Resolution; j++)
	{
		for (float i = 0; i < m_Resolution; i++)
		{
			for (float i2 = 0; i2 < 6; i2++)
			{
				glm::vec3 point = glm::vec3(
					cube[vertexIndex + 0],
					cube[vertexIndex + 1],
					cube[vertexIndex + 2]
				);
				glm::vec3 pointOnUnitSphere = glm::normalize(point);
				glm::vec3 p = m_ShapeGenerator->CalculatePointOnPlanet(pointOnUnitSphere);

				cube[vertexIndex + 0] = p.x;
				cube[vertexIndex + 1] = p.y;
				cube[vertexIndex + 2] = p.z;

				vertexIndex += 10;
			}
		}
	}
	return cube;
}

float* Sandbox3D::FillNormals(float* sphere)
{
	int vertexIndex = 0;
	for (float j = 0; j < m_Resolution; j++)
	{
		for (float i = 0; i < m_Resolution; i++)
		{
			glm::vec3 a00(sphere[vertexIndex + 0 + 0], sphere[vertexIndex + 0 + 1], sphere[vertexIndex + 0 + 2]);
			glm::vec3 a10(sphere[vertexIndex + 10 + 0], sphere[vertexIndex + 10 + 1], sphere[vertexIndex + 10 + 2]);
			glm::vec3 a11(sphere[vertexIndex + 20 + 0], sphere[vertexIndex + 20 + 1], sphere[vertexIndex + 20 + 2]);
			glm::vec3 a01(sphere[vertexIndex + 50 + 0], sphere[vertexIndex + 50 + 1], sphere[vertexIndex + 50 + 2]);

			glm::vec3 u1 = a11 - a00;
			glm::vec3 v1 = a10 - a00;

			glm::vec3 u2 = a01 - a00;
			glm::vec3 v2 = a11 - a00;

			glm::vec3 norm1 = glm::cross(v1, u1);
			glm::vec3 norm2 = glm::cross(v2, u2);

			sphere[vertexIndex + 0 + 7] = norm1.x;
			sphere[vertexIndex + 0 + 8] = norm1.y;
			sphere[vertexIndex + 0 + 9] = norm1.z;

			sphere[vertexIndex + 10 + 7] = norm1.x;
			sphere[vertexIndex + 10 + 8] = norm1.y;
			sphere[vertexIndex + 10 + 9] = norm1.z;

			sphere[vertexIndex + 20 + 7] = norm1.x;
			sphere[vertexIndex + 20 + 8] = norm1.y;
			sphere[vertexIndex + 20 + 9] = norm1.z;

			sphere[vertexIndex + 30 + 7] = norm2.x;
			sphere[vertexIndex + 30 + 8] = norm2.y;
			sphere[vertexIndex + 30 + 9] = norm2.z;

			sphere[vertexIndex + 40 + 7] = norm2.x;
			sphere[vertexIndex + 40 + 8] = norm2.y;
			sphere[vertexIndex + 40 + 9] = norm2.z;

			sphere[vertexIndex + 50 + 7] = norm2.x;
			sphere[vertexIndex + 50 + 8] = norm2.y;
			sphere[vertexIndex + 50 + 9] = norm2.z;
			vertexIndex += 60;
		}
	}
	return sphere;
}

uint32_t * Sandbox3D::FillIndices()
{
	uint32_t* indices = new uint32_t[m_Resolution * m_Resolution * 2 * 3];
	int indicesIndex = 0;
	int vertexIndex = 0;
	for (int j = 0; j < m_Resolution; j++)
	{
		for (int i = 0; i < m_Resolution; i++)
		{
			indices[indicesIndex++] = vertexIndex++;
			indices[indicesIndex++] = vertexIndex++;
			indices[indicesIndex++] = vertexIndex++;
			indices[indicesIndex++] = vertexIndex++;
			indices[indicesIndex++] = vertexIndex++;
			indices[indicesIndex++] = vertexIndex++;
		}
	}
	return indices;
}

void Sandbox3D::ConstructSphereFace(Can::Object * m_SphereFace, float xOffSet, float yOffSet, const glm::vec3 & localUp)
{
	m_SphereFace->VA = Can::VertexArray::Create();

	if (m_SphereFace->Vertices != nullptr)
		delete[] m_SphereFace->Vertices;

	m_SphereFace->Vertices = FillNormals(ShapeSphere(CreateSphere(localUp, xOffSet, yOffSet)));

	m_SphereFace->VB = Can::VertexBuffer::Create(m_SphereFace->Vertices, sizeof(float) * m_Resolution * m_Resolution * 2 * 3 * (3 + 4 + 3), true);
	m_SphereFace->VB->SetLayout({
	   { Can::ShaderDataType::Float3, "a_Position"},
	   { Can::ShaderDataType::Float4, "a_Color"},
	   { Can::ShaderDataType::Float3, "a_Normal"}
		});
	m_SphereFace->VA->AddVertexBuffer(m_SphereFace->VB);

	if (m_SphereFace->Indices != nullptr)
		delete[] m_SphereFace->Indices;
	m_SphereFace->Indices = FillIndices();

	m_SphereFace->IB = Can::IndexBuffer::Create(m_SphereFace->Indices, m_Resolution * m_Resolution * 2 * 3);
	m_SphereFace->VA->SetIndexBuffer(m_SphereFace->IB);

	m_SphereFace->S = Can::Shader::Create("assets/shaders/Cube.glsl");

	m_SphereFace->S->Bind();
	m_SphereFace->S->SetInt("u_Texture", 0);
	m_SphereFace->S->SetFloat3("u_LightPos", { 4.0f, 0.0f, 0.0f });

	m_SphereFace->position = { 0.0f, 0.0f, -4.0f };
	m_SphereFace->scale = { 0.5f, 0.5f, 0.5f };
	m_SphereFace->transform = glm::translate(glm::mat4(1.0f), m_SphereFace->position) * glm::scale(glm::mat4(1.0f), m_SphereFace->scale);
}

Can::Object * Sandbox3D::ConstructObject(const std::string & shaderPath, const std::string & texturePath, std::vector<glm::vec3> & vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> & normals)
{
	Can::Object* object = new Can::Object();
	object->VA = Can::VertexArray::Create();

	int vSize = vertices.size();
	int size = vSize * (3 + 2 + 3);
	float* m_Vertices = new float[size];

	for (int i = 0; i < vSize; i++)
	{
		int index = i * 8;
		m_Vertices[index + 0] = vertices[i].x;
		m_Vertices[index + 1] = vertices[i].y;
		m_Vertices[index + 2] = vertices[i].z;
		m_Vertices[index + 3] = uvs[i].x;
		m_Vertices[index + 4] = uvs[i].y;
		m_Vertices[index + 5] = normals[i].x;
		m_Vertices[index + 6] = normals[i].y;
		m_Vertices[index + 7] = normals[i].z;
	}

	object->VB = Can::VertexBuffer::Create(m_Vertices, sizeof(float) * size, true);
	object->VB->SetLayout({
	   { Can::ShaderDataType::Float3, "a_Position"},
	   { Can::ShaderDataType::Float2, "a_UV"},
	   { Can::ShaderDataType::Float3, "a_Normal"}
		});

	object->VA->AddVertexBuffer(object->VB);

	uint32_t* m_Indices = new uint32_t[vSize];

	for (int i = 0; i < vSize; i++)
	{
		m_Indices[i] = i;
	}

	object->IB = Can::IndexBuffer::Create(m_Indices, vSize);
	object->VA->SetIndexBuffer(object->IB);

	object->T = Can::Texture2D::Create(texturePath);
	object->S = Can::Shader::Create(shaderPath);

	object->S->Bind();
	object->S->SetInt("u_Texture", 0);
	object->S->SetFloat3("u_LightPos", { 4.0f, 0.0f, 0.0f });

	SetTransform(object, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

	delete[] m_Vertices;
	delete[] m_Indices;

	return object;
}

bool Sandbox3D::loadOBJ(const char* path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);

		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;
}

void Sandbox3D::UpdateSphere()
{
	int size = m_FaceResolution * m_FaceResolution;
	int index = 0;
	for (int k = 0; k < 6; k++)
	{
		for (int i = 0; i < m_FaceResolution; i++)
		{
			for (int j = 0; j < m_FaceResolution; j++)
			{
				UpdateSphereFace(
					m_SphereFaces[index],
					m_LocalUps[k],
					(1.0f * i) / m_FaceResolution,
					(1.0f * j) / m_FaceResolution
				);
				index++;
			}
		}
	}
}

void Sandbox3D::UpdateSphereFace(Can::Object * m_SphereFace, const glm::vec3 & localUp, float xOffSet, float yOffSet)
{
	if (!m_SphereFace->isEnabled)
		return;
	delete[] m_SphereFace->Vertices;
	m_SphereFace->Vertices = FillNormals(ShapeSphere(CreateSphere(localUp, xOffSet, yOffSet)));

	int vertexCount = m_Resolution * m_Resolution * 2 * 3 * (3 + 4 + 3);
	m_SphereFace->VB->Bind();
	m_SphereFace->VB->ReDo(m_SphereFace->Vertices, sizeof(float) * vertexCount);
	m_SphereFace->VB->Unbind();


	int IndexCount = m_Resolution * m_Resolution * 2 * 3;
	delete[] m_SphereFace->Indices;
	m_SphereFace->Indices = FillIndices();

	m_SphereFace->IB->ReDo(m_SphereFace->Indices, IndexCount);
	m_SphereFace->VA->SetIndexBuffer(m_SphereFace->IB);
	m_SphereFace->IB->Unbind();
}

void Sandbox3D::SetTransform(Can::Object * object, glm::vec3 pos, glm::vec3 scale)
{
	object->position = pos;
	object->scale = scale;
	object->transform = glm::translate(glm::mat4(1.0f), object->position) * glm::scale(glm::mat4(1.0f), object->scale);
}