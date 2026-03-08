#include "HardwareScanner.h"
#include <thread>

#ifdef _WIN32
    #include <windows.h>
    #include <dxgi.h>
    #include <intrin.h> 
    #pragma comment(lib, "dxgi.lib")
#else
    // ONLY included on Linux/Vulkan where they are used
    #include <vector>   // For storing Vulkan devices
    #include <fstream>  // For reading /proc/cpuinfo
    #include <sstream>  // For parsing CPU info
    #include <vulkan/vulkan.h>
    #include <unistd.h>
#endif

namespace SpecSync {

    HardwareProfile HardwareScanner::RunBaselineScan() {
        HardwareProfile Profile = {};
        ScanRAM(Profile);
        ScanCPU(Profile);
        ScanGPU(Profile);
        return Profile;
    }

    void HardwareScanner::ScanRAM(HardwareProfile& OutProfile) {
#ifdef _WIN32
        MEMORYSTATUSEX MemStatus;
        MemStatus.dwLength = sizeof(MEMORYSTATUSEX);
        if (GlobalMemoryStatusEx(&MemStatus)) {
            OutProfile.TotalSystemRAM_MB = static_cast<size_t>(MemStatus.ullTotalPhys / (1024 * 1024));
        }
#else
        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        OutProfile.TotalSystemRAM_MB = (pages * page_size) / (1024 * 1024);
#endif
    }

    void HardwareScanner::ScanCPU(HardwareProfile& OutProfile) {
        OutProfile.CpuLogicalCores = std::thread::hardware_concurrency();

#ifdef _WIN32
        // Get CPU Brand String using CPUID
        int cpuInfo[4] = { -1 };
        char cpuBrandString[48];
        __cpuid(cpuInfo, 0x80000002);
        memcpy(cpuBrandString, cpuInfo, sizeof(cpuInfo));
        __cpuid(cpuInfo, 0x80000003);
        memcpy(cpuBrandString + 16, cpuInfo, sizeof(cpuInfo));
        __cpuid(cpuInfo, 0x80000004);
        memcpy(cpuBrandString + 32, cpuInfo, sizeof(cpuInfo));
        OutProfile.CpuName = cpuBrandString;
#else
        // Read CPU name from /proc/cpuinfo
        std::ifstream cpuinfo("/proc/cpuinfo");
        std::string line;
        while (std::getline(cpuinfo, line)) {
            if (line.find("model name") != std::string::npos) {
                size_t pos = line.find(": ");
                if (pos != std::string::npos) {
                    OutProfile.CpuName = line.substr(pos + 2);
                    break;
                }
            }
        }
#endif
    }

    void HardwareScanner::ScanGPU(HardwareProfile& OutProfile) {
#ifdef _WIN32
        IDXGIFactory* pFactory = nullptr;
        if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
            IDXGIAdapter* pAdapter = nullptr;
            if (pFactory->EnumAdapters(0, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
                DXGI_ADAPTER_DESC desc;
                if (SUCCEEDED(pAdapter->GetDesc(&desc))) {
                    char defChar = ' ';
                    char ch[128];
                    WideCharToMultiByte(CP_ACP, 0, desc.Description, -1, ch, 128, &defChar, NULL);
                    OutProfile.GpuName = std::string(ch);
                    OutProfile.TotalVRAM_MB = desc.DedicatedVideoMemory / (1024 * 1024);
                }
                pAdapter->Release();
            }
            pFactory->Release();
        }
#else
        // Vulkan implementation for Linux to get GPU info
        VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
        VkInstance instance;
        if (vkCreateInstance(&createInfo, nullptr, &instance) == VK_SUCCESS) {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
            if (deviceCount > 0) {
                std::vector<VkPhysicalDevice> devices(deviceCount);
                vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
                VkPhysicalDeviceProperties props;
                vkGetPhysicalDeviceProperties(devices[0], &props);
                OutProfile.GpuName = props.deviceName;
                
                VkPhysicalDeviceMemoryProperties memProps;
                vkGetPhysicalDeviceMemoryProperties(devices[0], &memProps);
                for (uint32_t i = 0; i < memProps.memoryHeapCount; i++) {
                    if (memProps.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                        OutProfile.TotalVRAM_MB = memProps.memoryHeaps[i].size / (1024 * 1024);
                        break;
                    }
                }
            }
            vkDestroyInstance(instance, nullptr);
        }
#endif
        OutProfile.bIsLegacyGPU = (OutProfile.TotalVRAM_MB < 6144);
    }
}