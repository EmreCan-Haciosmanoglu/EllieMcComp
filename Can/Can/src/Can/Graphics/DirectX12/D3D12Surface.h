#pragma once
#include "D3D12CommonHeaders.h"
#include "D3D12Resources.h"

namespace Can::graphics::d3d12
{
	class d3d12_surface
	{
	public:
		constexpr static u32 buffer_count{ 3 };
		constexpr static DXGI_FORMAT default_back_buffer_format{ DXGI_FORMAT_R8G8B8A8_UNORM_SRGB };

		d3d12_surface() = default;

		explicit d3d12_surface(platform::window window)
			: _window{ window }
		{
			assert(_window.handle());
		}

		DISABLE_COPY(d3d12_surface);
		d3d12_surface(d3d12_surface&& o)
			: _swap_chain{ o._swap_chain }
			, _window{ o._window }
			, _current_bb_index{ o._current_bb_index }
			, _allow_tearing{ o._allow_tearing }
			, _present_flags{ o._present_flags }
			, _viewport{ o._viewport }
			, _scissor_rect{ o._scissor_rect }
			, _light_culling_id{ o._light_culling_id }
		{
			for (u32 i{ 0 }; i < buffer_count; ++i)
			{
				_render_target_data[i].resource = o._render_target_data[i].resource;
				_render_target_data[i].rtv = o._render_target_data[i].rtv;
			}

			d3d12_surface::reset_to_default(&o);
		}
		d3d12_surface& operator=(d3d12_surface&& o)
		{
			assert(this != &o);
			if (this != &o)
			{
				release();
				d3d12_surface::move(this, &o);
			}
		}

		~d3d12_surface() { release(); }

		void create_swap_chain(IDXGIFactory7* factory, ID3D12CommandQueue* cmd_queue);
		void present() const;
		void resize();

		[[nodiscard]] constexpr u32 width() const { return (u32)_viewport.Width; }
		[[nodiscard]] constexpr u32 height() const { return (u32)_viewport.Height; }
		[[nodiscard]] constexpr ID3D12Resource* const back_buffer() const { return _render_target_data[_current_bb_index].resource; }
		[[nodiscard]] constexpr D3D12_CPU_DESCRIPTOR_HANDLE rtv() const { return _render_target_data[_current_bb_index].rtv.cpu; }
		[[nodiscard]] constexpr const D3D12_VIEWPORT& viewport() const { return _viewport; }
		[[nodiscard]] constexpr const D3D12_RECT& scissor_rect() const { return _scissor_rect; }
		[[nodiscard]] constexpr id::id_type light_culling_id() const { return _light_culling_id; }

		static void move(d3d12_surface* dest, d3d12_surface* src);
		static void reset_to_default(d3d12_surface* dest);

	private:
		void finalize();
		void release();

		struct render_target_data
		{
			ID3D12Resource* resource{ nullptr };
			descriptor_handle rtv{};
		};

		IDXGISwapChain4* _swap_chain{ nullptr };
		render_target_data _render_target_data[buffer_count]{};
		platform::window   _window{};
		mutable u32        _current_bb_index{ 0 };
		u32                _allow_tearing{ 0 };
		u32                _present_flags{ 0 };
		D3D12_VIEWPORT     _viewport{};
		D3D12_RECT         _scissor_rect{};
		id::id_type        _light_culling_id{ id::invalid_id };
	};
}