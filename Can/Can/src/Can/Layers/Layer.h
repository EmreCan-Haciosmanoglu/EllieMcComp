#pragma once

#include "Can/Core.h"
#include "Can/Events/Event.h"
#include "Can/Core/TimeStep.h"

namespace Can::Layer
{
	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual bool OnUpdate(TimeStep ts) { return false; }
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event::Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	public:
		bool m_Enabled;
	};
}