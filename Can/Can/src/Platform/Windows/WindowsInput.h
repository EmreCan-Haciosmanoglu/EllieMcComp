#pragma once

#include "Can/Input.h"
#include "Can/KeyCodes.h"
#include "Can/MouseCodes.h"

namespace Can::Platform::Windows
{
	class WindowsInput : public Input
	{
	public:

	protected:
		virtual bool IsKeyPressedImp(KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImp(MouseCode button) override;
		virtual float GetMouseXImp() override;
		virtual float GetMouseYImp() override;
		virtual std::pair<float, float> GetMousePosImp() override;
	};
}