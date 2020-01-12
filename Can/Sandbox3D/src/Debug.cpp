#include "canpch.h"
#include "Debug.h"
#include "SandBox3DApp.h"

Debug::Debug(Sandbox3D* parent)
	:m_Parent(parent)
{
}

void Debug::OnAttach()
{
}

void Debug::OnDetach()
{
}

void Debug::OnUpdate(Can::TimeStep ts)
{
}

void Debug::OnEvent(Can::Event::Event& event)
{
}

void Debug::OnImGuiRender()
{
	ImGui::Begin("Settings");
	bool isChanged = false;

	isChanged = isChanged || ImGui::Checkbox("Index1", &(m_Parent->m_FaceEnabled[0]));
	isChanged = isChanged || ImGui::Checkbox("Index2", &(m_Parent->m_FaceEnabled[1]));
	isChanged = isChanged || ImGui::Checkbox("Index3", &(m_Parent->m_FaceEnabled[2]));
	isChanged = isChanged || ImGui::Checkbox("Index4", &(m_Parent->m_FaceEnabled[3]));
	isChanged = isChanged || ImGui::Checkbox("Index5", &(m_Parent->m_FaceEnabled[4]));
	isChanged = isChanged || ImGui::Checkbox("Index6", &(m_Parent->m_FaceEnabled[5]));

	int* resolution = m_Parent->GetResolutionPtr();
	bool isResolutionChanged = ImGui::SliderInt("Resolution", resolution, 1, 100);

	NoiseLayer** layers = m_Parent->GetShapeGenerator()->GetShapeSettings()->GetNoiseLayers();
	int size = m_Parent->GetShapeGenerator()->GetShapeSettings()->GetLayerCount();
	for (int i = 0; i < size; i++)
	{
		//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::BeginChild(i + 1,ImVec2(800, 200),true);
		NoiseSettings* settings = layers[i]->GetNoiseSettings();
		isChanged = isChanged || ImGui::Checkbox("Enabled", layers[i]->GetEnabledPtr());
		isChanged = isChanged || ImGui::Checkbox("Mask", layers[i]->GetMaskPtr());
		isChanged = isChanged || ImGui::SliderFloat("Strength", settings->GetStrengthPtr(), 0.0f, 1.0f);
		isChanged = isChanged || ImGui::SliderInt("NumLayer", settings->GetNumLayerPtr(), 1, 5);
		isChanged = isChanged || ImGui::SliderFloat("Base Roughness", settings->GetBaseRoughnessPtr(), 0.0f, 5.0f);
		isChanged = isChanged || ImGui::SliderFloat("Roughness", settings->GetRoughnessPtr(), 0.0f, 5.0f);
		isChanged = isChanged || ImGui::SliderFloat("Persistence", settings->GetPersistencePtr(), 0.0f, 1.0f);
		isChanged = isChanged || ImGui::SliderFloat("Min Value", settings->GetMinValuePtr(), 0.0f, 2.0f);
		isChanged = isChanged || ImGui::SliderFloat3("Center X", (float*)settings->GetCenterPtr(), 0.0f, 10.0f);
		ImGui::EndChild();
	}

	if (isChanged || isResolutionChanged)
		m_Parent->UpdateSphere(isResolutionChanged);


	ImGui::End();

}
