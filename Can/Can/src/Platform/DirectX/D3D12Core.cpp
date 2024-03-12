#include "canpch.h"
#include "D3D12Core.h"

using Microsoft::WRL;

namespace Can
{
    ID3D12Device8* main_device { nullptr };
    IDXGIFactory7* dxgi_factory { nullptr };
    constexpr D3D_FEATURE_LEVEL minimum_feature_level = D3D_FEATURE_LEVEL_11_0;

    static bool failed_init()
    {
        shutdown();
        return false;
    }

    static IDXGIAdapter4* determine_main_adapter()
    {
        IDXGIAdapter4* adapter { nullptr };
        u32 i = 0;
        while(true)
        {
            auto result = dxgi_factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
            if (result == DXGI_ERROR_NOT_FOUND) break;
            if (SUCCEEDED(D3D12CreateDevice(adapter, minimum_feature_level, __uuidof(ID3D12Device), nullptr))) return adapter;
            if (adapter) 
            {
                adapter->Release();
                adapter = nullptr;
            }
            ++i;
        } 
        return nullptr;
    }

    static D3D_FEATURE_LEVEL get_maximum_feature_level(IDXGIAdapter4* adapter)
    {
        constexpr D3D_FEATURE_LEVEL feature_levels[4] {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_12_1
        };

        D3D12_FEATURE_DATA_FEATURE_LEVELS feature_level_info {};
        feature_level_info.NumFeatureLevels = _countof(feature_levels);
        feature_level_info.pFeatureLevelsRequested = feature_levels;

        ComPtr<ID3D12Device> device;
        auto result = D3D12CreateDevice(adapter, minimum_feature_level, IID_PPV_ARGS(&device));
        if (FAILED(result)) return D3D_FEATURE_LEVEL_11_0;
        if (!device) return D3D_FEATURE_LEVEL_11_0;
        auto result2 = device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &feature_level_info, sizeof(feature_level_info));
        if (FAILED(result2)) return D3D_FEATURE_LEVEL_11_0;
        return feature_level_info.MaxSupportedFeatureLevel;
    }
    bool initialize()
    {
        if (main_device) shutdown();
        u32 dxgi_factory_flags { 0 };
#ifdef CAN_DEBUG
        {
            ComPtr<ID3D12Debug3> debug_interface;
            auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface));
            if (FAILED(result)) return failed_init();
            if (!debug_interface) return failed_init();
            debug_interface->EnableDebugLayer();
            dxgi_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
        }
#endif
        auto result = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&dxgi_factory));
        if (FAILED(result)) return failed_init();

        ComPtr<IDXGIAdapter4> main_adapter;
        auto determined_main_adapter = determine_main_adapter();
        if (determined_main_adapter) return failed_init();
        main_adapter.Attach(determined_main_adapter);
        if (!main_adapter) return failed_init();

        D3D_FEATURE_LEVEL maximum_feature_level = get_maximum_feature_level(main_adapter.Get());
        assert(maximum_feature_level >= minimum_feature_level);
        if( maximum_feature_level < minimum_feature_level) return failed_init();

        result = D3D12CreateDevice(main_adapter.Get(), maximum_feature_level, IID_PPV_ARGS(&main_adapter));
        if (FAILED(result)) return failed_init();

        main_device->SetName(L"Main Device");
        
#ifdef CAN_DEBUG
        {
            ComPtr<ID3D12InfoQueue> info_queue;
            auto result = main_device->QueryInterface(IID_PPV_ARGS(&info_queue));
            if (FAILED(result)) return failed_init();
            if (!info_queue) return failed_init();
            
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CURRUPTION, true);
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        }
#endif
        return true;
    }

    bool shutdown()
    {
        if (dxgi_factory) 
        {
            dxgi_factory->Release();
            dxgi_factory = nullptr;
        }
        if (main_device) 
        {
#ifdef CAN_DEBUG
        {
            ComPtr<ID3D12InfoQueue> info_queue;
            auto result = main_device->QueryInterface(IID_PPV_ARGS(&info_queue));
            if (FAILED(result)) return false;
            if (!info_queue) return false;
            
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CURRUPTION, false);
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);
            info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, false);
            
            ComPtr<ID3D12DebugDevice> debug_device;
            result = main_device->QueryInterface(IID_PPV_ARGS(&debug_device));
            if (FAILED(result)) return false;
            if (!debug_device) return false;
            main_device->Release();
            main_device = nullptr;
            result = debug_device->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL)
            if (FAILED(result)) return false;
        }
#else
            main_device->Release();
            main_device = nullptr;
#endif
        }
    }
}
