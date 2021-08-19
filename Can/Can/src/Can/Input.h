#pragma once
#include "canpch.h"

#include "Can/Core.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Can
{
	class  Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImp(key); }

		inline static bool IsMouseButtonPressed(MouseCode  button) { return s_Instance->IsMouseButtonPressedImp(button); }
		inline static f32 GetMouseX() { return s_Instance->GetMouseXImp(); }
		inline static f32 GetMouseY() { return s_Instance->GetMouseYImp(); }
		inline static std::pair<u32, u32> get_mouse_pos() { return s_Instance->get_mouse_position_imp(); }
		inline static std::pair<f32, f32> get_mouse_pos_float() { return s_Instance->get_mouse_position_imp_float(); }
	protected:
		virtual bool IsKeyPressedImp(KeyCode  keycode) = 0;
		virtual bool IsMouseButtonPressedImp(MouseCode  button) = 0;
		virtual f32 GetMouseXImp() = 0;
		virtual f32 GetMouseYImp() = 0;
		virtual std::pair<u32, u32> get_mouse_position_imp() = 0;
		virtual std::pair<f32, f32> get_mouse_position_imp_float() = 0;
	private:
		static Scope <Input> s_Instance;
	};
}