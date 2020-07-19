#include "canpch.h"
#include "OnClickCallbackComponent.h"

namespace Can
{
	OnClickCallbackComponent::OnClickCallbackComponent(const std::function<void()>& callback)
		:Callback(callback)
	{
	}
}