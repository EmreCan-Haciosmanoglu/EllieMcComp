#include "canpch.h"
#include "OnChangeCallbackComponent.h"

namespace Can
{
	OnChangeCallbackComponent::OnChangeCallbackComponent(const std::function<void()>& callback)
		:Callback(callback)
	{
	}
}