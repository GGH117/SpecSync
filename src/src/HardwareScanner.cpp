#include "HardwareScanner.h"
#include <dxgi.h>
#include <wrl/client.h> 

#pragma comment(lib, "dxgi.lib")

namespace SpecSync 
{
    HardwareProfile HardwareScanner::ScanHostSystem() 
    {
        HardwareProfile Profile;

        // 1. RAM Detection (Source Requirement: RAM total) [cite: 1, 15]
        MEMORYSTATUSEX MemStatus;
        MemStatus.dwLength = sizeof(MEMORYSTATUSEX);
        if (GlobalMemoryStatusEx(&MemStatus))
        {
            Profile.SystemRAM_GB = static_cast<uint32_t>(MemStatus.ullTotalPhys / (1024 * 1024 * 1024));
        }

        // 2. CPU Detection (Source Requirement: CPU model/cores) [cite: 1, 13]
        SYSTEM_INFO SysInfo;
        GetSystemInfo(&SysInfo);
        Profile.CPU_Cores = SysInfo.dwNumberOfProcessors;
        Profile.CPU_Model = "x64 Architecture"; 

        // 3. GPU Detection (Source Requirement: GPU model/VRAM) [cite: 1, 14]
        Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
        if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf())))) 
        {
            Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
            if (SUCCEEDED(pFactory->EnumAdapters(0, pAdapter.GetAddressOf()))) 
            {
                DXGI_ADAPTER_DESC AdapterDesc;
                if (SUCCEEDED(pAdapter->GetDesc(&AdapterDesc)))
                {
                    // Convert WideChar to std::string for the GPU Model
                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, AdapterDesc.Description, -1, NULL, 0, NULL, NULL);
                    std::string gpuName(size_needed, 0);
                    WideCharToMultiByte(CP_UTF8, 0, AdapterDesc.Description, -1, &gpuName[0], size_needed, NULL, NULL);
                    
                    Profile.GPU_Model = gpuName;
                    Profile.GPU_VRAM_MB = static_cast<uint32_t>(AdapterDesc.DedicatedVideoMemory / (1024 * 1024));
                }
            }
        }
        return Profile;
    }

    bool HardwareProfile::IsMinimumReqMet() const
    {
        return (CPU_Cores >= 4 && SystemRAM_GB >= 8 && GPU_VRAM_MB >= 2048);
    }
}