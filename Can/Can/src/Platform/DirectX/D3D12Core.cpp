#include "canpch.h"
#include "D3D12Core.h"

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
        if ( result == DXGI_ERROR_NOT_FOUND) break;
        if (SUCCEEDED(D3D12CreateDevice(adapter, minimum_feature_level, __uuidof(ID3D12Device), nullptr))) return adapter;
        if (adapter) adapter->Release();
        ++i;
      } 
      return nullptr;
    }

    bool initialize()
    {
      if (main_device) shutdown();
      u32 dxgi_factory_flags { 0 };
#ifdef CAN_DEBUG
      dxgi_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
#endif
      auto result = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&dxgi_factory));
      if (FAILED(result)) return failed_init();

      using Microsoft::WRL;
      ComPtr<IDXGIAdapter4> main_adapter;
      auto determined_main_adapter = determine_main_adapter();
      main_adapter.Attach(determined_main_adapter);
      if (!main_adapter) return failed_init();
    }

    bool shutdown()
    {
    }
}
