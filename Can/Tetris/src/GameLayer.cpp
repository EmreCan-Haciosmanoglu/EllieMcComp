#include "canpch.h"
#include "GameLayer.h"
#include "Player.h"

GameLayer::GameLayer()
	:Layer("2048-Extended Game")
	, m_CameraController(1920.0f / 1080.0f, 12.0f)
	, m_PlayerCount(1)
	, m_PlayerLeft(m_PlayerCount)
	, m_Brain(new NeuralNetwork(new int[3]{ STATE_SIZE, STATE_SIZE / 2, 5 }, 3, 0.02f))
	, m_Players(new Player* [m_PlayerCount])
{
	for (int i = 0; i < m_PlayerCount; i++)
		m_Players[i] = new Player(m_GameWidth, m_GameHeight, m_Brain, this);
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
	ImGui::Begin("Details", 0, 1);
	ImGui::Text("Player Count:  %03d", m_PlayerCount);
	ImGui::Text("Player Left:   %03d", m_PlayerLeft);
	ImGui::Text("Generation    #%03d", m_Generation);
	ImGui::Text("Current Max    %03d", m_CurrentMaxPoint);
	ImGui::Text("Max Point      %03d", m_MaxPoint);
	ImGui::SliderInt("Index", &m_CurrenPlayerIndex, 0, m_PlayerCount - 1);
	ImGui::End();
	if (label)
	{
		ImGui::Begin("Decision", 0, 1);
		ImGui::Text("Last Training Percentage %3.2f", m_LastPerc);
		ImGui::Text("Labeled Data Size %3d", m_LabeledData.size());
		ImGui::SliderInt("Label Index", &m_LabelIndex, 0, m_UnlabeledData.size() - 1);
		ImGui::Text("");
		const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

		static float PassButtonWidth = 100.0f;
		float pos = ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Pass"))
		{
			LabeTheData({ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		}
		PassButtonWidth = ImGui::GetItemRectSize().x;

		static float RotateButtonWidth = 100.0f;
		pos += PassButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Rotate"))
		{
			LabeTheData({ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f });
		}
		RotateButtonWidth = ImGui::GetItemRectSize().x;

		static float DownButtonWidth = 100.0f;
		pos += RotateButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Down"))
		{
			LabeTheData({ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
		}
		DownButtonWidth = ImGui::GetItemRectSize().x;

		static float LeftButtonWidth = 100.0f;
		pos += DownButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Left"))
		{
			LabeTheData({ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f });
		}
		LeftButtonWidth = ImGui::GetItemRectSize().x;

		static float RightButtonWidth = 100.0f;
		pos += LeftButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Right"))
		{
			LabeTheData({ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f });
		}
		RightButtonWidth = ImGui::GetItemRectSize().x;

		if (ImGui::Button("New Brain"))
		{
			NewBrain();
		}
		if (ImGui::Button("Next Generation"))
		{
			NextGeneration();
		}
		ImGui::InputInt("Testing Count", &m_TestingCount, 100, 1000);
		if (ImGui::Button("Test"))
		{
			Test();
		}
		ImGui::InputInt("Training Count", &m_TrainingCount, 100, 1000);
		if (ImGui::Button("Train"))
		{
			Train();
		}
		ImGui::End();
	}
}

void GameLayer::AddData(std::array<float, STATE_SIZE> data)
{
	if (m_LabeledData.find(data) != m_LabeledData.end())
		return;
	for (auto& element : m_UnlabeledData)
	{
		if (element.first == data)
		{
			element.second = element.second + 1;
			return;
		}
	}
	m_UnlabeledData.push_back(std::make_pair(data, 1));
}

void GameLayer::UpdateGame(float ts)
{
	if (m_PlayerLeft <= 0 && b_First)
	{
		b_First = false;
		std::sort(m_UnlabeledData.begin(), m_UnlabeledData.end(), compFunctor);
		label = true;
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
	m_Players[m_CurrenPlayerIndex]->Draw({
			(m_GameWidth * -4.0f) / 2.0f + 1,
			m_GameHeight / -2.0f
		});
	if (label)
	{
		DrawToLabel();
	}
}

void GameLayer::DrawToLabel()
{
	std::array<float, STATE_SIZE> state = m_UnlabeledData[m_LabelIndex].first;
	std::vector<std::vector<bool>> blockX = GetBlock(state[STATE_SIZE - 4] - 1);
	glm::vec2 offset = {
			((m_GameWidth * 1.0f) / 2.0f) + 1,
			m_GameHeight / -2.0f
	};
	int index = 0;
	for (int i = 0; i < m_GameHeight; i++)
	{
		for (int j = 0; j < m_GameWidth; j++)
		{
			Can::Renderer2D::DrawQuad(
				{ offset.x + j, offset.y + i },
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);
			if (state[index] == 1)
				Can::Renderer2D::DrawQuad(
					{ offset.x + j, offset.y + i, 0.011f },
					{ 0.9f, 0.9f },
					{ 0.3f, 0.8f, 0.2f, 1.0f }
			);
			else
				Can::Renderer2D::DrawQuad(
					{ offset.x + j, offset.y + i, 0.01f },
					{ 0.9f, 0.9f },
					{ 0.9f, 0.9f, 0.9f, 1.0f }
			);
			index++;
		}
	}

	std::vector<std::vector<bool>> block;
	glm::vec2 pos = { m_GameWidth + 2, (m_GameHeight * 1.0f) / 4.0f - 1.0f };
	block = GetBlock(state[index + 0] - 1);
	for (int i = 0; i < block.size(); i++)
	{
		for (int j = 0; j < block[0].size(); j++)
		{
			Can::Renderer2D::DrawQuad(
				{
					offset.x + j + pos.x,
					offset.y - i + pos.y
				},
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);

			if (block[i][j])
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f
					},
					{ 0.9f, 0.9f },
					{ 0.3f, 0.2f, 0.8f, 1.0f }
			);
			else
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f },
					{ 0.9f, 0.9f },
					{ 0.9f, 0.9f, 0.9f, 1.0f }
			);
		}
	}
	pos = { m_GameWidth + 2, (m_GameHeight * 2.0f) / 4.0f - 1.0f };
	block = GetBlock(state[index + 1] - 1);
	for (int i = 0; i < block.size(); i++)
	{
		for (int j = 0; j < block[0].size(); j++)
		{
			Can::Renderer2D::DrawQuad(
				{
					offset.x + j + pos.x,
					offset.y - i + pos.y
				},
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);

			if (block[i][j])
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f
					},
					{ 0.9f, 0.9f },
					{ 0.3f, 0.2f, 0.8f, 1.0f }
			);
			else
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f },
					{ 0.9f, 0.9f },
					{ 0.9f, 0.9f, 0.9f, 1.0f }
			);
		}
	}
	pos = { m_GameWidth + 2, (m_GameHeight * 3.0f) / 4.0f - 1.0f };
	block = GetBlock(state[index + 2] - 1);
	for (int i = 0; i < block.size(); i++)
	{
		for (int j = 0; j < block[0].size(); j++)
		{
			Can::Renderer2D::DrawQuad(
				{
					offset.x + j + pos.x,
					offset.y - i + pos.y
				},
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);

			if (block[i][j])
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f
					},
					{ 0.9f, 0.9f },
					{ 0.3f, 0.2f, 0.8f, 1.0f }
			);
			else
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f },
					{ 0.9f, 0.9f },
					{ 0.9f, 0.9f, 0.9f, 1.0f }
			);
		}
	}
	pos = { m_GameWidth + 2, (m_GameHeight * 4.0f) / 4.0f - 1.0f };
	block = GetBlock(state[index + 3] - 1);
	for (int i = 0; i < block.size(); i++)
	{
		for (int j = 0; j < block[0].size(); j++)
		{
			Can::Renderer2D::DrawQuad(
				{
					offset.x + j + pos.x,
					offset.y - i + pos.y
				},
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);

			if (block[i][j])
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f
					},
					{ 0.9f, 0.9f },
					{ 0.3f, 0.2f, 0.8f, 1.0f }
			);
			else
				Can::Renderer2D::DrawQuad(
					{
						offset.x + j + pos.x,
						offset.y - i + pos.y,
						0.011f },
					{ 0.9f, 0.9f },
					{ 0.9f, 0.9f, 0.9f, 1.0f }
			);
		}
	}

	int blockXh = blockX.size();
	int blockXw = blockX[0].size();
	for (int i = 0; i < blockXh; i++)
	{
		for (int j = 0; j < blockXw; j++)
		{
			if (blockX[i][j])
			{
				if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] - 1 == 0)
					Can::Renderer2D::DrawQuad(
						{
							offset.x + j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1] - 1,
							offset.y - i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2] - 1,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] - 1 == 1)
					Can::Renderer2D::DrawQuad(
						{
							offset.x + i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1] - 1,
							offset.y + j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2] - 1 - blockXw + 1,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] - 1 == 2)
					Can::Renderer2D::DrawQuad(
						{
							offset.x - j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1] - 1 + blockXw - 1,
							offset.y + i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2] - 1 - blockXh + 1,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] - 1 == 3)
					Can::Renderer2D::DrawQuad(
						{
							offset.x - i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1] - 1 + blockXh - 1,
							offset.y - j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2] - 1,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
			}
		}
	}

}

void GameLayer::LabeTheData(std::array<float, 5> label)
{
	std::array<float, STATE_SIZE> data = m_UnlabeledData[m_LabelIndex].first;
	m_UnlabeledData.erase(m_UnlabeledData.begin() + m_LabelIndex);
	m_LabelIndex = 0;

	m_LabeledData.insert(std::pair<std::array<float, STATE_SIZE>, std::array<float, 5>>(data, label));

}

void GameLayer::NextGeneration()
{
	int* points = new int[m_PlayerCount];
	for (unsigned int i = 0; i < m_PlayerCount; i++)
	{
		points[i] = m_Players[i]->GetPoint();
	}

	int* maxElement = std::max_element(points, points + m_PlayerCount);
	_int64 index = std::distance(points, maxElement);

	m_MaxPoint = std::max(m_MaxPoint, m_Players[index]->GetPoint());
	m_CurrentMaxPoint = 0;
	m_Generation++;
	delete[] points;
	for (int i = 0; i < m_PlayerCount; i++)
	{
		delete m_Players[i];
		m_Players[i] = new Player(m_GameWidth, m_GameHeight, m_Brain, this);
	}
	m_PlayerLeft = m_PlayerCount;
	label = false;
	b_First = true;
}

void GameLayer::Test()
{
	float correct = 0;
	float all = 0;

	if (m_LabeledData.size() <= 0)
		return;
	for (int i = 0; i < m_TestingCount; i++)
	{
		int rnd = Can::Utility::Random::Integer(0, m_LabeledData.size());
		auto it = m_LabeledData.begin();
		std::advance(it, rnd);

		float* state = new float[STATE_SIZE];
		for (size_t i = 0; i < STATE_SIZE; i++)
		{
			state[i] = it->first[i];
		}
		Matrix* input = new Matrix(STATE_SIZE, 1, state);
		Matrix* result = m_Brain->FeedForward(input);

		float A[5] = {
			result->data[0],
			result->data[1],
			result->data[2],
			result->data[3],
			result->data[4]
		};
		float* maxElement = std::max_element(A, A + 5);
		_int64 dist = std::distance(A, maxElement);

		if (it->second[dist] == 1)
			correct++;
		all++;
		delete result;
	}
	m_LastPerc = (correct * 100.0f) / all;
}

void GameLayer::Train()
{
	if (m_LabeledData.size() <= 0)
		return;
	for (int i = 0; i < m_TrainingCount; i++)
	{
		int rnd = Can::Utility::Random::Integer(0, m_LabeledData.size());
		auto it = m_LabeledData.begin();
		std::advance(it, rnd);
		float in[STATE_SIZE] = {};
		for (size_t i = 0; i < STATE_SIZE; i++)
		{
			in[i] = it->first[i];
		}
		float ta[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		for (size_t i = 0; i < 5; i++)
		{
			ta[i] = it->second[i];
		}

		Matrix* input = new Matrix(STATE_SIZE, 1, in);
		Matrix* target = new Matrix(5, 1, ta);

		m_Brain->Train(input, target);
	}
}

void GameLayer::NewBrain()
{
	if (m_Brain != nullptr)
		delete m_Brain;
	m_Brain = new NeuralNetwork(new int[3]{ STATE_SIZE, STATE_SIZE / 2, 5 }, 3, 0.02f);
}
