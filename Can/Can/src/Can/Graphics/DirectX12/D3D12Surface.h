#pragma once
#include "D3D12CommonHeaders.h"
#include "D3D12Resources.h"

namespace Can::graphics::d3d12
{
	class d3d12_surface
	{
	public:
		explicit d3d12_surface(platform::window window)
			: _window{ window }
		{
			assert(_window.handle());
		}

		~d3d12_surface() { release(); }

		void create_swap_chain(IDXGIFactory7* factory, ID3D12CommandQueue* cmd_queue, DXGI_FORMAT format);
		void present() const;
		void resize();

		[[nodiscard]] constexpr u32 width() const { return (u32)_viewport.Width; }
		[[nodiscard]] constexpr u32 height() const { return (u32)_viewport.Height; }
		[[nodiscard]] constexpr ID3D12Resource* const back_buffer() const { return _render_target_data[_current_bb_index].resource; }
		[[nodiscard]] constexpr D3D12_CPU_DESCRIPTOR_HANDLE rtv() const { return _render_target_data[_current_bb_index].rtv.cpu; }
		[[nodiscard]] constexpr const D3D12_VIEWPORT& viewport() const { return _viewport; }
		[[nodiscard]] constexpr const D3D12_RECT& scissor_rect() const { return _scissor_rect; }

	private:
		void finalize();
		void release();

		struct render_target_data
		{
			ID3D12Resource* resource{ nullptr };
			descriptor_handle rtv{};
		};

		IDXGISwapChain4* _swap_chain{ nullptr };
		render_target_data _render_target_data[frame_buffer_count]{};
		platform::window _window;
		mutable u32 _current_bb_index{ 0 };
		D3D12_VIEWPORT _viewport{};
		D3D12_RECT _scissor_rect{};
	};
}