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
	int faceCount = m_Parent->GetFaceCount();
	Can::Object** faces = m_Parent->GetFaces();
	for (int i = 0; i < faceCount; i++)
	{
		char integer_string[6];
		sprintf(integer_string, "%d", i);

		char other_string[26] = "Index: ";
		strcat(other_string, integer_string);
		isChanged = isChanged || ImGui::Checkbox(other_string, &(faces[i]->isEnabled));
	}

	int* resolution = m_Parent->GetResolutionPtr();
	isChanged = isChanged || ImGui::SliderInt("Resolution", resolution, 1, 100);

	NoiseLayer** layers = m_Parent->GetShapeGenerator()->GetShapeSettings()->GetNoiseLayers();
	int layercount = m_Parent->GetShapeGenerator()->GetShapeSettings()->GetLayerCount();
	for (int i = 0; i < layercount; i++)
	{
		//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::BeginChild(i + 1,ImVec2(800, 200),true);
		NoiseSettings* settings = layers[i]->GetNoiseSettings();
		isChanged = isChanged || ImGui::Checkbox("Enabled", layers[i]->GetEnabledPtr());
		isChanged = isChanged || ImGui::Checkbox("Mask", layers[i]->GetMaskPtr());
		isChanged = isChanged || ImGui::SliderFloat("Strength", settings->GetStrengthPtr(), 0.0f, 10.0f);
		isChanged = isChanged || ImGui::SliderInt("NumLayer", settings->GetNumLayerPtr(), 1, 5);
		isChanged = isChanged || ImGui::SliderFloat("Base Roughness", settings->GetBaseRoughnessPtr(), 0.0f, 5.0f);
		isChanged = isChanged || ImGui::SliderFloat("Roughness", settings->GetRoughnessPtr(), 0.0f, 5.0f);
		isChanged = isChanged || ImGui::SliderFloat("Persistence", settings->GetPersistencePtr(), 0.0f, 1.0f);
		isChanged = isChanged || ImGui::SliderFloat("Min Value", settings->GetMinValuePtr(), 0.0f, 2.0f);
		isChanged = isChanged || ImGui::SliderFloat3("Center X", (float*)settings->GetCenterPtr(), 0.0f, 10.0f);
		ImGui::EndChild();
	}

	if (isChanged)
		m_Parent->UpdateSphere();


	ImGui::End();

}
