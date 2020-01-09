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
	bool isChanged = ImGui::SliderInt("Resolution", resolution, 1, 100);

	NoiseSettings* settings = m_Parent->GetShapeGenerator()->GetShapeSettings()->GetNoiseSettings();

	bool isStrengthChanged = ImGui::SliderFloat("Strength", settings->GetStrengthPtr(), 0.0f, 1.0f);
	bool isRoughnessChanged = ImGui::SliderFloat("Roughness", settings->GetRoughnessPtr(), 0.0f, 1.0f);

	if (isChanged || isStrengthChanged || isRoughnessChanged)
		m_Parent->UpdateSphere();


	ImGui::End();

}
