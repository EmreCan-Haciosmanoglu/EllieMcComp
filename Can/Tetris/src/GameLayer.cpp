#include "canpch.h"
#include "GameLayer.h"
#include "Player.h"
#include <random>

int myrandom(int i) { return std::rand() % i; }

GameLayer::GameLayer()
	:Layer("2048-Extended Game")
	, m_CameraController(1920.0f / 1080.0f, 12.0f)
	, m_PlayerCount(1)
	, m_PlayerLeft(m_PlayerCount)
	, m_Brain(new NeuralNetwork(new int[3]{ STATE_SIZE, STATE_SIZE, 5 }, 3, 0.005f))
	, m_Players(new Player* [m_PlayerCount])
{
	for (int i = 0; i < m_PlayerCount; i++)
		m_Players[i] = new Player(m_GameWidth, m_GameHeight, m_Brain, this);
}

void GameLayer::OnUpdate(Can::TimeStep ts)
{
	UpdateGame(ts);
	if (b_IsTraining)
		Train();

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
	ImGui::Text("Generation  #%03d", m_Generation);
	ImGui::Text("Current Max   %03d", m_CurrentMaxPoint);
	ImGui::Text("Max Point      %03d", m_MaxPoint);
	ImGui::End();
	if (label)
	{
		ImGui::Begin("Decision", 0, 1);
		ImGui::Text("Last Training Percentage %3.3f", m_LastPerc);
		ImGui::Text("Labeled Data Size %3d", m_LabeledData.size());

		ImGui::Text("L_Pass:    %03d", m_LabelCounts[0]);
		ImGui::Text("L_Rotate: %03d", m_LabelCounts[1]);
		ImGui::Text("L_Down:  %03d", m_LabelCounts[2]);
		ImGui::Text("L_Left:      %03d", m_LabelCounts[3]);
		ImGui::Text("L_Right:   %03d", m_LabelCounts[4]);

		ImGui::SliderInt("Label Index", &m_LabelIndex, 0, (int)(m_UnlabeledData.size()) - 1);
		ImGui::Text("");
		const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

		static float PassButtonWidth = 100.0f;
		float pos = ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Pass"))
		{
			LabelTheData({ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		}
		PassButtonWidth = ImGui::GetItemRectSize().x;

		static float RotateButtonWidth = 100.0f;
		pos += PassButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Rotate"))
		{
			LabelTheData({ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f });
		}
		RotateButtonWidth = ImGui::GetItemRectSize().x;

		static float DownButtonWidth = 100.0f;
		pos += RotateButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Down"))
		{
			LabelTheData({ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
		}
		DownButtonWidth = ImGui::GetItemRectSize().x;

		static float LeftButtonWidth = 100.0f;
		pos += DownButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Left"))
		{
			LabelTheData({ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f });
		}
		LeftButtonWidth = ImGui::GetItemRectSize().x;

		static float RightButtonWidth = 100.0f;
		pos += LeftButtonWidth + ItemSpacing;
		ImGui::SameLine(pos);
		if (ImGui::Button("Right"))
		{
			LabelTheData({ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f });
		}
		RightButtonWidth = ImGui::GetItemRectSize().x;
		ImGui::Checkbox("Custom Neural Network", &b_CustomNN);
		if (b_CustomNN)
		{
			static int layerC = 3;
			static int layers[] = {
				STATE_SIZE,
				STATE_SIZE,
				STATE_SIZE
			};
			static int lr = 2;

			ImGui::SliderInt("Learning Rate", &lr, 1, 100);
			ImGui::SliderInt("Layer Count", &layerC, 3, 5);
			if (layerC == 3)
			{
				ImGui::InputInt("Layer", layers, 1, 10);
			}
			else if (layerC == 4)
			{
				ImGui::InputInt2("Layer", layers);
			}
			else
			{
				ImGui::InputInt3("Layer", layers);
			}

			if (ImGui::Button("New Brain"))
			{
				NewBrain(
					lr / 100.0f,
					layerC,
					layerC == 3 ?
					new int[3]{ STATE_SIZE, layers[0],5 } : layerC == 4 ?
					new int[4]{ STATE_SIZE, layers[0], layers[1], 5 } :
					new int[5]{ STATE_SIZE, layers[0], layers[1], layers[2], 5 },
					false
				);
			}
		}
		else
		{
			if (ImGui::Button("New Brain"))
			{
				NewBrain(0.0f, 0, nullptr, true);
			}
		}
		if (ImGui::Button("Next Generation"))
		{
			NextGeneration();
		}
		if (ImGui::Button("Test"))
		{
			Test();
		}
		if (b_IsTraining)
			ImGui::Text("Current Epoch : #%3d", m_CurrentEpoch);
		ImGui::InputInt("Training Count", &m_Epoch, 10, 100);
		if (ImGui::Button("Train") && m_LabeledData.size() > 0)
		{
			b_IsTraining = true;
			ShuffleData();
		}
		ImGui::End();
	}
}

void GameLayer::AddData(std::array<float, STATE_SIZE> data)
{
	if (m_LabeledData.find(data) != m_LabeledData.end())
		return;
	auto it = std::find(m_UnlabeledData.begin(), m_UnlabeledData.end(), data);
	if (it == m_UnlabeledData.end())
		m_UnlabeledData.push_back(data);
}

void GameLayer::UpdateGame(float ts)
{
	if (m_PlayerLeft <= 0 && b_First)
	{
		b_First = false;
		label = true;
	}
	else
	{
		int* points = new int[m_PlayerCount];
		for (unsigned int i = 0; i < (unsigned int)m_PlayerCount; i++)
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
	for (unsigned int i = 0; i < (unsigned int)m_PlayerCount; i++)
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
	std::array<float, STATE_SIZE> state = m_UnlabeledData[m_LabelIndex];
	std::vector<std::vector<bool>> blockX = GetBlock(state[STATE_SIZE - 4]);
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


	glm::vec2 pos = { m_GameWidth + 2, (m_GameHeight * 1.0f) / 4.0f - 1.0f };
	std::vector<std::vector<bool>> block1 = GetBlock(state[(size_t)(index)]);
	for (int i = 0; i < block1.size(); i++)
	{
		for (int j = 0; j < block1[0].size(); j++)
		{
			Can::Renderer2D::DrawQuad(
				{
					offset.x + j + pos.x,
					offset.y - i + pos.y
				},
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);

			if (block1[i][j])
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
	std::vector<std::vector<bool>> block2 = GetBlock(state[(size_t)(index + 1)]);
	for (int i = 0; i < block2.size(); i++)
	{
		for (int j = 0; j < block2[0].size(); j++)
		{
			Can::Renderer2D::DrawQuad(
				{
					offset.x + j + pos.x,
					offset.y - i + pos.y
				},
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);

			if (block2[i][j])
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
	std::vector<std::vector<bool>> block3 = GetBlock(state[(size_t)(index + 2)]);
	for (int i = 0; i < block3.size(); i++)
	{
		for (int j = 0; j < block3[0].size(); j++)
		{
			Can::Renderer2D::DrawQuad(
				{
					offset.x + j + pos.x,
					offset.y - i + pos.y
				},
				{ 1.0f, 1.0f },
				{ 0.05f, 0.05f, 0.05f, 1.0f }
			);

			if (block3[i][j])
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
				if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] == 0)
					Can::Renderer2D::DrawQuad(
						{
							offset.x + j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1],
							offset.y - i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2],
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] == 1)
					Can::Renderer2D::DrawQuad(
						{
							offset.x - i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1] + blockXh - 1,
							offset.y - j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2],
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] == 2)
					Can::Renderer2D::DrawQuad(
						{
							offset.x - j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1] + blockXw - 1,
							offset.y + i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2] - blockXh + 1,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
				else if (state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 3] == 3)
					Can::Renderer2D::DrawQuad(
						{
							offset.x + i + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 1],
							offset.y + j + state[(size_t)index + (size_t)BLOCK_QUEUE_SIZE + 2] - blockXw + 1,
							0.011f
						},
						{ 0.9f, 0.9f },
						{ 0.3f, 0.2f, 0.8f, 1.0f }
				);
			}
		}
	}
}

void GameLayer::LabelTheData(std::array<float, 5> label)
{
	if (m_UnlabeledData.size() < 3)
		return;

	if (label[0] == 1)
		m_LabelCounts[0]++;
	else if (label[1] == 1)
		m_LabelCounts[1]++;
	else if (label[2] == 1)
		m_LabelCounts[2]++;
	else if (label[3] == 1)
		m_LabelCounts[3]++;
	else
		m_LabelCounts[4]++;

	std::array<float, STATE_SIZE> data = m_UnlabeledData[m_LabelIndex];
	m_UnlabeledData.erase(m_UnlabeledData.begin() + m_LabelIndex);

	if (m_LabelIndex >= m_UnlabeledData.size() - 1)
		m_LabelIndex--;

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
	int labelCount = m_LabeledData.size();
	if (labelCount <= 0)
		return;
	auto it = m_LabeledData.begin();
	for (int i = 0; i < labelCount; i++)
	{
		float* state = GetRandomState(it);

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

		delete result;
		delete state;

		std::advance(it, 1);
	}
	m_LastPerc = (correct * 100.0f) / labelCount;
}

void GameLayer::Train()
{
	int labelCount = m_LabeledData.size();
	auto it = m_TrainingDataIndexVector.at(m_CurrentEpoch * labelCount);

	for (int i = 0; i < labelCount; i++)
	{
		float* in = GetRandomState(it);

		float ta[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		for (size_t i = 0; i < 5; i++)
		{
			ta[i] = it->second[i];
		}

		Matrix* input = new Matrix(STATE_SIZE, 1, in);
		Matrix* target = new Matrix(5, 1, ta);

		m_Brain->Train(input, target);

		delete in;
		std::advance(it, 1);
	}
	m_CurrentEpoch++;
	if (m_Epoch <= m_CurrentEpoch)
	{
		b_IsTraining = false;
		m_CurrentEpoch = 0;
	}
}

void GameLayer::NewBrain(float learningRate, int layerCount, int* nodes, bool default)
{
	if (default)
		m_Brain = new NeuralNetwork(new int[3]{ STATE_SIZE, STATE_SIZE, 5 }, 3, 0.02f);
	else
		m_Brain = new NeuralNetwork(nodes, layerCount, learningRate);

}

void GameLayer::ShuffleData()
{
	m_TrainingDataIndexVector.clear();
	auto& it = m_LabeledData.begin();
	int size = m_LabeledData.size();

	for (int j = 0; j < size; j++)
	{
		for (int i = 0; i < m_Epoch; i++)
		{
			m_TrainingDataIndexVector.push_back(it);
		}
		std::advance(it, 1);
	}
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(m_TrainingDataIndexVector.begin(), m_TrainingDataIndexVector.end(), g);
}

float* GameLayer::GetRandomState(std::map<std::array<float, STATE_SIZE>, std::array<float, 5>>::iterator it)
{
	float* state = new float[STATE_SIZE];
	for (size_t i = 0; i < GAME_SIZE; i++)
	{
		state[i] = it->first[i];
	}
	for (size_t i = 0; i < BLOCK_QUEUE_SIZE; i++)
	{
		state[GAME_SIZE + i] = (it->first[GAME_SIZE + i] + 1.0f) / 4.0f;
	}
	state[GAME_SIZE + BLOCK_QUEUE_SIZE + 0] = (it->first[GAME_SIZE + BLOCK_QUEUE_SIZE + 0] + 1.0f) / (8.0f);
	state[GAME_SIZE + BLOCK_QUEUE_SIZE + 1] = (it->first[GAME_SIZE + BLOCK_QUEUE_SIZE + 1] + 1.0f) / (m_GameWidth + 1.0f);
	state[GAME_SIZE + BLOCK_QUEUE_SIZE + 2] = (it->first[GAME_SIZE + BLOCK_QUEUE_SIZE + 2] + 1.0f) / (m_GameHeight + 1.0f);
	state[GAME_SIZE + BLOCK_QUEUE_SIZE + 3] = (it->first[GAME_SIZE + BLOCK_QUEUE_SIZE + 3] + 1.0f) / (4.0f);

	return state;
}
