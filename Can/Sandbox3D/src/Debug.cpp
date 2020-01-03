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
	bool isChanged = ImGui::SliderInt("Resolution", resolution, 1, 25);
	if (isChanged)
		m_Parent->UpdateSphere(*resolution);
	ImGui::End();
}
