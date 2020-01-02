#include "canpch.h"
#include "GameLayer.h"

GameLayer::GameLayer()
	:Layer("2048-Extended Game")
	, m_CameraController(1920.0f / 1080.0f, 10.0f)
	, m_FishCount(500)
	, m_FishLeft(m_FishCount)
	, m_Scene(new Scene(30.0f, 10.0f,this))
	, m_Fish(new Fish* [m_FishCount])
{
	for (int i = 0; i < m_FishCount; i++)
		m_Fish[i] = new Fish(
			3.0f,
			5.0f,
			-13.0f,
			5.0f,
			-5.0f,
			new NeuralNetwork(new int[3]{ 8, 6, 3 }, 3, 0.05f),
			this
		);
}

void GameLayer::OnUpdate(Can::TimeStep ts)
{
	UpdateGame(ts);

	Can::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	DrawGame(10.0f);

	Can::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Can::Event::Event & event)
{

}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("Details");
	ImGui::Text("Fish Left:  %03d", m_FishLeft);
	ImGui::Text("Generation #%03d", m_Generation);
	ImGui::Text("Current Max %03d", m_CurrentMaxPoint);
	ImGui::Text("Max Point   %03d", m_MaxPoint);
	ImGui::End();
}

void GameLayer::AddPoint()
{
	m_CurrentMaxPoint++;
	for (int i = 0; i < m_FishCount; i++)
	{
		if (m_Fish[i]->IsDead())
			continue;
		m_Fish[i]->AddPoint();
	}
}

void GameLayer::UpdateGame(float ts)
{
	m_Scene->Update(ts);
	if (m_FishLeft <= 0)
	{
		NeuralNetwork** brains = new NeuralNetwork * [m_FishCount];
		int* points = new int[m_FishCount];
		int total = 1;
		for (unsigned int i = 0; i < m_FishCount; i++)
		{
			brains[i] = m_Fish[i]->GetBrain();
			points[i] = m_Fish[i]->GetPoints();
			total += points[i];
		}

		int* maxElement = std::max_element(points, points + m_FishCount);
		_int64 index = std::distance(points, maxElement);
		m_MaxPoint = std::max(m_MaxPoint, points[index]);

		brains = NeuralNetwork::Generate(brains, points, total, m_FishCount, m_FishCount);
		brains = NeuralNetwork::Mutate(brains, 5, m_FishCount);
		m_FishLeft = m_FishCount;
		m_Generation++;
		m_CurrentMaxPoint = 0;
		for (unsigned int i = 0; i < m_FishCount; i++)
			delete m_Fish[i];

		for (unsigned int i = 0; i < m_FishCount; i++)
		{
			m_Fish[i] = new Fish(3.0f, 5.0f, -13.0f, 5.0f, -5.0f, brains[i], this);
		}
		delete[] points;

		delete m_Scene;
		m_Scene = new Scene(30.0f, 10.0f, this);
	}
	else
	{
		int n = 7;
		float* state = new float[n];
		Obstacle obs1 = m_Scene->GetObstacle(0);
		state[0] = obs1.x1;
		state[1] = obs1.w;
		state[2] = obs1.y1;
		state[3] = obs1.h1;
		state[4] = obs1.y2;
		state[5] = obs1.h2;
		state[6] = 10.0f;
		for (int i = 0; i < m_FishCount; i++)
		{
			if (m_Fish[i]->IsDead())
				continue;
			m_Fish[i]->Update(ts, state, n);
		}
		delete[] state;
	}
}

void GameLayer::DrawGame(float height)
{
	m_Scene->Draw();
	for (int i = 0; i < m_FishCount; i++)
	{
		if (m_Fish[i]->IsDead())
			continue;
		m_Fish[i]->Draw(height,i);
	}
}
