#include "canpch.h"
#include "D3D12Helpers.h"
#include "D3D12Core.h"
#include "D3D12Upload.h"

namespace Can::graphics::d3d12::d3dx
{
	namespace
	{

	}

	void transition_resource(
		id3d12_graphics_command_list* cmd_list,
		ID3D12Resource* resource,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after,
		D3D12_RESOURCE_BARRIER_FLAGS flags /*= D3D12_RESOURCE_BARRIER_FLAG_NONE*/,
		u32 subresource /*= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES*/
	) {
		D3D12_RESOURCE_BARRIER barier{};
		barier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barier.Flags = flags;
		barier.Transition.pResource = resource;
		barier.Transition.StateBefore = before;
		barier.Transition.StateAfter = after;
		barier.Transition.Subresource = subresource;

		cmd_list->ResourceBarrier(1, &barier);
	}

	ID3D12RootSignature* create_root_signature(const D3D12_ROOT_SIGNATURE_DESC1& desc)
	{
		D3D12_VERSIONED_ROOT_SIGNATURE_DESC versioned_desc{};
		versioned_desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		versioned_desc.Desc_1_1 = desc;

		HRESULT hr{ S_OK };
		Microsoft::WRL::ComPtr<ID3DBlob> signature_blob{ nullptr };
		Microsoft::WRL::ComPtr<ID3DBlob> error_blob{ nullptr };
		if (FAILED(hr = D3D12SerializeVersionedRootSignature(&versioned_desc, &signature_blob, &error_blob)))
		{
			DEBUG_OP(const char* error_msg{ error_blob ? (const char*)error_blob->GetBufferPointer() : "" });
			DEBUG_OP(OutputDebugStringA(error_msg));
			DEBUG_OP(OutputDebugStringA("\n"));
			return nullptr;
		}


		ID3D12RootSignature* signature{ nullptr };
		DXCall(hr = core::device()->CreateRootSignature(
			0,
			signature_blob->GetBufferPointer(),
			signature_blob->GetBufferSize(),
			IID_PPV_ARGS(&signature)
		));
		if (FAILED(hr))
		{
			core::release(signature);
		}

		return signature;
	}
	ID3D12PipelineState* create_pipeline_state(D3D12_PIPELINE_STATE_STREAM_DESC desc)
	{
		assert(desc.pPipelineStateSubobjectStream && desc.SizeInBytes);
		ID3D12PipelineState* pso{ nullptr };
		DXCall(core::device()->CreatePipelineState(&desc, IID_PPV_ARGS(&pso)));
		assert(pso);
		return pso;
	}

	ID3D12PipelineState* create_pipeline_state(void* stream, u64 stream_size)
	{
		assert(stream && stream_size);
		D3D12_PIPELINE_STATE_STREAM_DESC desc{};
		desc.pPipelineStateSubobjectStream = stream;
		desc.SizeInBytes = stream_size;
		return create_pipeline_state(desc);
	}

	ID3D12Resource* create_buffer(
		const void* data,
		u32 buffer_size,
		bool is_cpu_accesible /* = false */,
		D3D12_RESOURCE_STATES state /* = D3D12_RESOURCE_STATE_COMMON */,
		D3D12_RESOURCE_FLAGS flags /* = D3D12_RESOURCE_FLAG_NONE */,
		ID3D12Heap* heap /* = nullptr */,
		u64 heap_offset /* = 0 */
	) {
		assert(buffer_size);

		D3D12_RESOURCE_DESC desc{};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = buffer_size;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc = { 1, 0 };
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = is_cpu_accesible ? D3D12_RESOURCE_FLAG_NONE : flags;

		assert(desc.Flags == D3D12_RESOURCE_FLAG_NONE ||
			desc.Flags == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		ID3D12Resource* resource{ nullptr };
		const D3D12_RESOURCE_STATES resource_state{ is_cpu_accesible ? D3D12_RESOURCE_STATE_GENERIC_READ : state };

		if (heap)
		{
			DXCall(core::device()->CreatePlacedResource(
				heap, heap_offset, &desc, resource_state,
				nullptr, IID_PPV_ARGS(&resource)
			));
		}
		else
		{
			DXCall(core::device()->CreateCommittedResource(
				is_cpu_accesible ? &heap_properties.upload_heap : &heap_properties.default_heap,
				D3D12_HEAP_FLAG_NONE, &desc, resource_state,
				nullptr, IID_PPV_ARGS(&resource)
			));
		}

		if (data)
		{
			if (is_cpu_accesible)
			{
				const D3D12_RANGE range{};
				void* cpu_address{ nullptr };
				DXCall(resource->Map(0, &range, reinterpret_cast<void**>(&cpu_address)));
				assert(cpu_address);
				memcpy(cpu_address, data, buffer_size);
				resource->Unmap(0, nullptr);
			}
			else
			{
				upload::d3d12_upload_context context{ buffer_size };
				memcpy(context.cpu_address(), data, buffer_size);
				context.command_list()->CopyResource(resource, context.upload_buffer());
				context.end_upload();
			}
		}

		assert(resource);
		return resource;
	}
}
