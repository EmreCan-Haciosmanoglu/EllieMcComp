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
		inline static float GetMouseX() { return s_Instance->GetMouseXImp(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImp(); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImp(); }
	protected:
		virtual bool IsKeyPressedImp(KeyCode  keycode) = 0;
		virtual bool IsMouseButtonPressedImp(MouseCode  button) = 0;
		virtual float GetMouseXImp() = 0;
		virtual float GetMouseYImp() = 0;
		virtual std::pair<float, float> GetMousePosImp() = 0;
	private:
		static Scope <Input> s_Instance;
	};
}