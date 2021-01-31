#include "canpch.h"
#include "OnDragCallbackComponent.h"

namespace Can
{
	OnDragCallbackComponent::OnDragCallbackComponent(const std::function<void()>& callback)
		:Callback(callback)
	{
	}
}