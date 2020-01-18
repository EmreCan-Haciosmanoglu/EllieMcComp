#include "canpch.h"
#include "GameLayer.h"
#include "Player.h"

GameLayer::GameLayer()
	:Layer("2048-Extended Game")
	, m_CameraController(1920.0f / 1080.0f, 10.0f)
	, m_PlayerCount(200)
	, m_PlayerLeft(m_PlayerCount)
	, m_Players(new Player* [m_PlayerCount])
{
	for (int i = 0; i < m_PlayerCount; i++)
		m_Players[i] = new Player(m_GameWidth, m_GameHeight, new NeuralNetwork(new int[3]{ 15, 8, 4 }, 3, 0.05f));
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
	ImGui::Text("Player Count:  %03d", m_PlayerCount);
	ImGui::Text("Player Left:   %03d", m_PlayerLeft);
	ImGui::Text("Generation    #%03d", m_Generation);
	ImGui::Text("Current Max    %03d", m_CurrentMaxPoint);
	ImGui::Text("Max Point      %03d", m_MaxPoint);
	ImGui::End();
}

void GameLayer::UpdateGame(float ts)
{
	if (m_PlayerLeft <= 0)
	{
		NeuralNetwork** brains = new NeuralNetwork * [m_PlayerCount];
		int* points = new int[m_PlayerCount];
		int total = 1;
		for (unsigned int i = 0; i < m_PlayerCount; i++)
		{
			brains[i] = m_Players[i]->GetBrain();
			points[i] = m_Players[i]->GetPoint();
			total += points[i];
		}

		int* maxElement = std::max_element(points, points + m_PlayerCount);
		_int64 index = std::distance(points, maxElement);
		m_MaxPoint = std::max(m_MaxPoint, points[index]);

		brains = NeuralNetwork::Generate(brains, points, total, m_PlayerCount, m_PlayerCount);
		brains = NeuralNetwork::Mutate(brains, 5, m_PlayerCount);
		m_PlayerLeft = m_PlayerCount;
		m_Generation++;
		m_CurrentMaxPoint = 0;
		for (unsigned int i = 0; i < m_PlayerCount; i++)
			delete m_Players[i];

		for (unsigned int i = 0; i < m_PlayerCount; i++)
		{
			m_Players[i] = new Player(m_GameWidth, m_GameHeight, brains[i]);
		}
		delete[] points;
	}
	else
	{
		for (int i = 0; i < m_PlayerCount; i++)
		{
			if (m_Players[i]->IsDead())
				continue;
			m_Players[i]->Update(ts);
		}
	}
}

void GameLayer::DrawGame()
{
	int* points = new int[m_PlayerCount];
	for (unsigned int i = 0; i < m_PlayerCount; i++)
	{
		points[i] = m_Players[i]->GetPoint();
	}

	int* maxElement = std::max_element(points, points + m_PlayerCount);
	_int64 index = std::distance(points, maxElement);

	m_Players[index]->Draw();
}
