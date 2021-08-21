#pragma once

#include "canpch.h"

#include "Can/Core.h"
#include "Can/Events/Event.h"

namespace Can
{
	struct  WindowProps
	{
		std::string Title = "Can Engine";
		unsigned int Width = 1280;
		unsigned int Height = 720;
	};

	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(Event::Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void set_resolution(u32 width, u32 height) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}