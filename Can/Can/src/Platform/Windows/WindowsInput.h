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
		virtual f32 GetMouseXImp() override;
		virtual f32 GetMouseYImp() override;
		virtual std::pair<u32, u32> get_mouse_position_imp() override;
		virtual std::pair<f32, f32> get_mouse_position_imp_float() override;
	};
}