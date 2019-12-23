#pragma once

#include "Layer.h"

#include "Can/Core.h"

#include "Can/Events/MouseEvent.h"
#include "Can/Events/KeyEvent.h"
#include "Can/Events/ApplicationEvent.h"
#include <xiosbase>



namespace Can::Layer
{
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}