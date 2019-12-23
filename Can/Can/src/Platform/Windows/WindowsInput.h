#pragma once

#include "Can/Input.h"

namespace Can::Platform::Windows
{
	class WindowsInput : public Input
	{
	public:

	protected:
		virtual bool IsKeyPressedImp(int keycode) override;
		virtual bool IsMouseButtonPressedImp(int button) override;
		virtual float GetMouseXImp() override;
		virtual float GetMouseYImp() override;
		virtual std::pair<float, float> GetMousePosImp() override;
	};
}