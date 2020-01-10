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

	int* resolution = m_Parent->GetResolutionPtr();
	bool isResolutionChanged = ImGui::SliderInt("Resolution", resolution, 1, 100);

	NoiseSettings* settings = m_Parent->GetShapeGenerator()->GetShapeSettings()->GetNoiseSettings();

	bool isStrengthChanged = ImGui::SliderFloat("Strength", settings->GetStrengthPtr(), 0.0f, 1.0f);
	bool isNumLayerChanged = ImGui::SliderInt("NumLayer", settings->GetNumLayerPtr(), 1, 5);
	bool isBaseRoughnessChanged = ImGui::SliderFloat("Base Roughness", settings->GetBaseRoughnessPtr(), 0.0f, 5.0f);
	bool isRoughnessChanged = ImGui::SliderFloat("Roughness", settings->GetRoughnessPtr(), 0.0f, 5.0f);
	bool isPersistenceChanged = ImGui::SliderFloat("Persistence", settings->GetPersistencePtr(), 0.0f, 1.0f);
	bool isMinValueChanged = ImGui::SliderFloat("Min Value", settings->GetMinValuePtr(), 0.0f, 2.0f);
	bool isCenterChanged = ImGui::SliderFloat3("Center X", (float *)settings->GetCenterPtr(), 0.0f, 10.0f);

	if (isMinValueChanged || isPersistenceChanged || isNumLayerChanged || isBaseRoughnessChanged || isCenterChanged || isResolutionChanged || isStrengthChanged || isRoughnessChanged)
		m_Parent->UpdateSphere(isResolutionChanged);


	ImGui::End();

}
