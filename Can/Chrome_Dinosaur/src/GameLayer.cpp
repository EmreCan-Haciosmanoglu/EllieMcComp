#include "canpch.h"
#include "GameLayer.h"

GameLayer::GameLayer()
	:Layer("2048-Extended Game")
	, m_CameraController(1920.0f / 1080.0f, 10.0f)
	, m_DinosaurCount(500)
	, m_DinosaurLeft(m_DinosaurCount)
	, m_Scene(new Scene(19.0f, 11.0f, m_CurrentSpeed,this))
	, m_Dinosaurs(new Dinosaur* [m_DinosaurCount])
{
	for (int i = 0; i < m_DinosaurCount; i++)
		m_Dinosaurs[i] = new Dinosaur(
			6.5f,
			8.0f,
			-14.0f,
			new NeuralNetwork(new int[3]{ 7, 6, 3 }, 3, 0.05f),
			this
		);
}

void GameLayer::OnUpdate(Can::TimeStep ts)
{
	UpdateGame(ts);

	Can::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	DrawGame();

	Can::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Can::Event::Event & event)
{

}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("Details");
	ImGui::Text("Dinosaur Left: %03d", m_DinosaurLeft);
	ImGui::Text("Generation    #%03d", m_Generation);
	ImGui::Text("Current Max    %03d", m_CurrentMaxPoint);
	ImGui::Text("Max Point      %03d", m_MaxPoint);
	ImGui::End();
}

void GameLayer::AddPoint()
{
	m_CurrentMaxPoint++;
	for (int i = 0; i < m_DinosaurCount; i++)
	{
		if (m_Dinosaurs[i]->IsDead())
			continue;
		m_Dinosaurs[i]->AddPoint();
	}
}

void GameLayer::UpdateGame(float ts)
{
	m_Scene->Update(ts);
	if (m_DinosaurLeft <= 0)
	{
		NeuralNetwork** brains = new NeuralNetwork * [m_DinosaurCount];
		int* points = new int[m_DinosaurCount];
		int total = 1;
		for (unsigned int i = 0; i < m_DinosaurCount; i++)
		{
			brains[i] = m_Dinosaurs[i]->GetBrain();
			points[i] = m_Dinosaurs[i]->GetPoints();
			total += points[i];
		}

		int* maxElement = std::max_element(points, points + m_DinosaurCount);
		_int64 index = std::distance(points, maxElement);
		m_MaxPoint = std::max(m_MaxPoint, points[index]);

		brains = NeuralNetwork::Generate(brains, points, total, m_DinosaurCount, m_DinosaurCount);
		brains = NeuralNetwork::Mutate(brains, 5, m_DinosaurCount);
		m_DinosaurLeft = m_DinosaurCount;
		m_Generation++;
		m_CurrentMaxPoint = 0;
		for (unsigned int i = 0; i < m_DinosaurCount; i++)
			delete m_Dinosaurs[i];

		for (unsigned int i = 0; i < m_DinosaurCount; i++)
		{
			m_Dinosaurs[i] = new Dinosaur(
				6.5f,
				8.0f,
				-14.0f,
				brains[i],
				this);
		}
		delete[] points;

		delete m_Scene;
		m_Scene = new Scene(19.0f, 11.0f, m_CurrentSpeed, this);
	}
	else
	{
		int n = 7;
		float* state = new float[n];
		Obstacle obs1 = m_Scene->GetObstacle(0);
		Obstacle obs2 = m_Scene->GetObstacle(1);
		state[0] = obs1.x;
		state[1] = obs1.w;
		state[2] = obs1.h;
		state[3] = obs2.x;
		state[4] = obs2.w;
		state[5] = obs2.h;
		state[6] = m_CurrentSpeed;
		for (int i = 0; i < m_DinosaurCount; i++)
		{
			if (m_Dinosaurs[i]->IsDead())
				continue;
			m_Dinosaurs[i]->Update(ts, state, n);
		}
		delete[] state;
	}
}

void GameLayer::DrawGame()
{
	m_Scene->Draw({ -8.0f, -1.0f,-0.01f });
	for (int i = 0; i < m_DinosaurCount; i++)
	{
		if (m_Dinosaurs[i]->IsDead())
			continue;
		m_Dinosaurs[i]->Draw({ -8.0f, -1.0f, 0.111f + i/1000.f });
	}
}
