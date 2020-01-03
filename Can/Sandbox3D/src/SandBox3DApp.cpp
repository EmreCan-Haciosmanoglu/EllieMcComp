#include "canpch.h"
#include "SandBox3DApp.h"
#include "Can/EntryPoint.h"

#define RESOLUTION 4
#define SCALE 2

Can::Application* Can::CreateApplication()
{
	return new Sandbox3D();
}

Sandbox3D::Sandbox3D()
	: m_Scene(new Scene(this))
	, m_Debug(new Debug(this))
	, m_Sphere(new Can::Object())
{
	ConstructSphere();
	Can::Renderer3D::AddObject(m_Sphere);
	PushLayer(m_Scene);
	PushOverlay(m_Debug);

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("assets/objects/test.obj", vertices, uvs, normals);
	if (res)
	{
		m_TestObject = ConstructObject("assets/shaders/Object.glsl", "assets/objects/Test.png", vertices, uvs, normals);
		Can::Renderer3D::AddObject(m_TestObject);
	}
}

Sandbox3D::~Sandbox3D()
{
}

void Sandbox3D::ConstructSphere()
{
	m_Sphere->VA = Can::VertexArray::Create();

	glm::vec3 localUps[6] = {
		{+1.0f, +0.0f, +0.0f},
		{-1.0f, +0.0f, +0.0f},
		{+0.0f, +1.0f, +0.0f},
		{+0.0f, -1.0f, +0.0f},
		{+0.0f, +0.0f, +1.0f},
		{+0.0f, +0.0f, -1.0f}
	};

	float* m_Vertices = new float[6 * RESOLUTION * RESOLUTION * 2 * 3 * (3 + 4 + 3)];

	int vertexIndex = 0;
	for (int k = 0; k < 6; k++)
	{
		glm::vec3 localUp = localUps[k];
		glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
		glm::vec3 axisB = glm::cross(localUp, axisA);
		for (float j = 0; j < RESOLUTION; j++)
		{
			for (float i = 0; i < RESOLUTION; i++)
			{
				{
					float x = i;
					float y = j;
					glm::vec2 percent = glm::vec2(x / RESOLUTION, y / RESOLUTION);
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
					glm::vec2 percent = glm::vec2(x / RESOLUTION, y / RESOLUTION);
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
					glm::vec2 percent = glm::vec2(x / RESOLUTION, y / RESOLUTION);
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
					glm::vec2 percent = glm::vec2(x / RESOLUTION, y / RESOLUTION);
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
					glm::vec2 percent = glm::vec2(x / RESOLUTION, y / RESOLUTION);
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
					glm::vec2 percent = glm::vec2(x / RESOLUTION, y / RESOLUTION);
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
		for (float j = 0; j < RESOLUTION; j++)
		{
			for (float i = 0; i < RESOLUTION; i++)
			{
				glm::vec3 a00(m_Vertices[vertexIndex + 0 + 0], m_Vertices[vertexIndex + 0 + 1], m_Vertices[vertexIndex + 0 + 2]);
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

	m_Sphere->VB = Can::VertexBuffer::Create(m_Vertices, sizeof(float) * 6 * RESOLUTION * RESOLUTION * 2 * 3 * (3 + 4 + 3), true);
	m_Sphere->VB->SetLayout({
	   { Can::ShaderDataType::Float3, "a_Position"},
	   { Can::ShaderDataType::Float4, "a_Color"},
	   { Can::ShaderDataType::Float3, "a_Normal"}
		});
	m_Sphere->VA->AddVertexBuffer(m_Sphere->VB);


	uint32_t * m_Indices = new uint32_t[6 * RESOLUTION * RESOLUTION * 2 * 3];
	int indicesIndex = 0;
	vertexIndex = 0;
	for (int k = 0; k < 6; k++)
	{
		for (int j = 0; j < RESOLUTION; j++)
		{
			for (int i = 0; i < RESOLUTION; i++)
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

	m_Sphere->IB = Can::IndexBuffer::Create(m_Indices, 6 * RESOLUTION * RESOLUTION * 2 * 3);
	m_Sphere->VA->SetIndexBuffer(m_Sphere->IB);

	m_Sphere->S = Can::Shader::Create("assets/shaders/Cube.glsl");

	m_Sphere->S->Bind();
	m_Sphere->S->SetInt("u_Texture", 0);
	m_Sphere->S->SetFloat3("u_LightPos", { 4.0f, 0.0f, 0.0f });

	delete[] m_Vertices;
	delete[] m_Indices;

	m_Sphere->position = { 0.0f, 0.0f, -4.0f };
	m_Sphere->scale = { 0.5f, 0.5f, 0.5f };
	m_Sphere->transform = glm::translate(glm::mat4(1.0f), m_Sphere->position) * glm::scale(glm::mat4(1.0f), m_Sphere->scale);

}

Can::Object* Sandbox3D::ConstructObject(const std::string& shaderPath, const std::string& texturePath, std::vector<glm::vec3> & vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> & normals)
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

	uint32_t * m_Indices = new uint32_t[vSize];

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

void Sandbox3D::UpdateSphere(int resolution)
{
	int vertexCount = 6 * resolution * resolution * 2 * 3 * (3 + 4 + 3);
	float* m_Vertices = new float[vertexCount];

	int IndexCount = 6 * resolution * resolution * 2 * 3;
	uint32_t * m_Indices = new uint32_t[IndexCount];

	{
		glm::vec3 localUps[6] = {
		   {+1.0f, +0.0f, +0.0f},
		   {-1.0f, +0.0f, +0.0f},
		   {+0.0f, +1.0f, +0.0f},
		   {+0.0f, -1.0f, +0.0f},
		   {+0.0f, +0.0f, +1.0f},
		   {+0.0f, +0.0f, -1.0f}
		};
		int vertexIndex = 0;
		for (int k = 0; k < 6; k++)
		{
			glm::vec3 localUp = localUps[k];
			glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
			glm::vec3 axisB = glm::cross(localUp, axisA);
			for (float j = 0; j < resolution; j++)
			{
				for (float i = 0; i < resolution; i++)
				{
					{
						float x = i;
						float y = j;
						glm::vec2 percent = glm::vec2(x / resolution, y / resolution);
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
						glm::vec2 percent = glm::vec2(x / resolution, y / resolution);
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
						glm::vec2 percent = glm::vec2(x / resolution, y / resolution);
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
						glm::vec2 percent = glm::vec2(x / resolution, y / resolution);
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
						glm::vec2 percent = glm::vec2(x / resolution, y / resolution);
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
						glm::vec2 percent = glm::vec2(x / resolution, y / resolution);
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
			for (float j = 0; j < resolution; j++)
			{
				for (float i = 0; i < resolution; i++)
				{
					glm::vec3 a00(m_Vertices[vertexIndex + 0 + 0], m_Vertices[vertexIndex + 0 + 1], m_Vertices[vertexIndex + 0 + 2]);
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
	}

	m_Sphere->VB->Bind();
	m_Sphere->VB->ReDo(m_Vertices, sizeof(float) * vertexCount);
	m_Sphere->VB->Unbind();

	{
		int indicesIndex = 0;
		int vertexIndex = 0;
		for (int k = 0; k < IndexCount; k++)
		{
			m_Indices[indicesIndex++] = vertexIndex++;
		}
	}

	m_Sphere->IB->ReDo(m_Indices, IndexCount);
	m_Sphere->VA->SetIndexBuffer(m_Sphere->IB);
	m_Sphere->IB->Unbind();

	delete[] m_Vertices;
	delete[] m_Indices;
}

void Sandbox3D::SetTransform(Can::Object* object, glm::vec3 pos, glm::vec3 scale)
{
	object->position = pos;
	object->scale = scale;
	object->transform = glm::translate(glm::mat4(1.0f), object->position) * glm::scale(glm::mat4(1.0f), object->scale);
}

