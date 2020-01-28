#include "canpch.h"
#include "GameLayer.h"
#include "Player.h"

GameLayer::GameLayer()
	:Layer("2048-Extended Game")
	, m_CameraController(1920.0f / 1080.0f, 12.0f)
	, m_PlayerCount(100)
	, m_PlayerLeft(m_PlayerCount)
	, m_Players(new Player* [m_PlayerCount])
{
	for (int i = 0; i < m_PlayerCount; i++)
		m_Players[i] = new Player(m_GameWidth, m_GameHeight, new NeuralNetwork(new int[3]{ 15, 8, 5 }, 3, 0.05f), this);
}

void GameLayer::OnUpdate(Can::TimeStep ts)
{
	/*static float time = 0.0f;
	time += ts;
	if (time > 0.05f)
	{
		if (Can::Input::IsKeyPressed(CAN_KEY_S))
		{
			m_Players[0]->MoveBlockDown();
		}
		else if (Can::Input::IsKeyPressed(CAN_KEY_A))
		{
			m_Players[0]->MoveHorizontal(true);
		}
		else if (Can::Input::IsKeyPressed(CAN_KEY_D))
		{
			m_Players[0]->MoveHorizontal(false);
		}
		else if (Can::Input::IsKeyPressed(CAN_KEY_SPACE))
		{
			m_Players[0]->Rotate();
		}
		time -= 0.05f;
	}*/
	UpdateGame(ts);

	Can::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1.0f });
	Can::RenderCommand::Clear();

	Can::Renderer2D::BeginScene(m_CameraController.GetCamera());

	DrawGame();

	Can::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Can::Event::Event & event)
{
	m_CameraController.OnEvent(event);
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("Details");
	ImGui::Text("Player Count:  %03d", m_PlayerCount);
	ImGui::Text("Player Left:   %03d", m_PlayerLeft);
	ImGui::Text("Generation    #%03d", m_Generation);
	ImGui::Text("Current Max    %03d", m_CurrentMaxPoint);
	ImGui::Text("Max Point      %03d", m_MaxPoint);
	ImGui::SliderInt("Current Player", &m_CurrenPlayerIndex, 0, m_PlayerCount-1);
	ImGui::Checkbox("Best of Provided", &m_IsChecked);
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
		brains = NeuralNetwork::Mutate(brains, 2, m_PlayerCount);
		m_PlayerLeft = m_PlayerCount;
		m_Generation++;
		m_CurrentMaxPoint = 0;
		for (unsigned int i = 0; i < m_PlayerCount; i++)
			delete m_Players[i];

		for (unsigned int i = 0; i < m_PlayerCount; i++)
		{
			m_Players[i] = new Player(m_GameWidth, m_GameHeight, brains[i], this);
		}
		delete[] points;
	}
	else
	{
		int* points = new int[m_PlayerCount];
		for (unsigned int i = 0; i < m_PlayerCount; i++)
		{
			points[i] = m_Players[i]->GetPoint();
		}

		int* maxElement = std::max_element(points, points + m_PlayerCount);
		_int64 index = std::distance(points, maxElement);
		m_CurrentMaxPoint = std::max(m_CurrentMaxPoint, points[index]);

		for (int i = 0; i < m_PlayerCount; i++)
		{
			if (m_Players[i]->IsDead())
				continue;
			m_Players[i]->Update(ts);
		}
		delete[] points;
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
	m_Players[m_IsChecked ? index : m_CurrenPlayerIndex]->Draw({ m_GameWidth / -2.0f,m_GameHeight / -2.0f });

}
