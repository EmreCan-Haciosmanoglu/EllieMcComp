#pragma once

namespace Can
{
	class GraphicsContext
	{
	public:
		GraphicsContext() {}
		virtual~GraphicsContext() { }

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
}