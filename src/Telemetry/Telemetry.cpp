#include "../../include/Telemetry.h" 
#include <numeric>

#ifdef _WIN32
    #include <windows.h>
    #include <dxgi1_4.h> // <--- This is the key to fixing the IDXGIAdapter3 errors
    #pragma comment(lib, "dxgi.lib")
#endif

namespace SpecSync {

    Telemetry::Telemetry() 
        : TimeSinceLastGpuQuery(0.0f) {
        CurrentFrame = {};
    }

    Telemetry::~Telemetry() {}

    void Telemetry::Tick(float DeltaTime) {
        CurrentFrame.DeltaTime = DeltaTime;
        
        // Calculate FPS (Safety check for Arithmatic Exception)
        CurrentFrame.CurrentFPS = (DeltaTime > 0.0f) ? (1.0f / DeltaTime) : 0.0f;

        // Keep a small history for averaging
        FpsHistory.push_back(CurrentFrame.CurrentFPS);
        if (FpsHistory.size() > 120) {
            FpsHistory.erase(FpsHistory.begin());
        }

        // Throttle hardware-intensive calls as to not hammer the GPU driver
        TimeSinceLastGpuQuery += DeltaTime;
        if (TimeSinceLastGpuQuery >= GpuQueryInterval) {
            UpdateHardwareStats();
            TimeSinceLastGpuQuery = 0.0f;
        }
    }

    float Telemetry::GetAverageFPS(int WindowSize) const {
        if (FpsHistory.empty()) return 0.0f;
        
        size_t actualSize = (FpsHistory.size() < (size_t)WindowSize) ? FpsHistory.size() : (size_t)WindowSize;
        float sum = std::accumulate(FpsHistory.end() - actualSize, FpsHistory.end(), 0.0f);
        return sum / static_cast<float>(actualSize);
    }

void Telemetry::UpdateHardwareStats() {
#ifdef _WIN32
        IDXGIFactory4* pFactory = nullptr;
        if (SUCCEEDED(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory))) {
            IDXGIAdapter3* pAdapter3 = nullptr;
            // Get the first adapter
            if (SUCCEEDED(pFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter3))) {
                DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
                // Query local VRAM
                if (SUCCEEDED(pAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo))) {
                    CurrentFrame.AvailableVRAM = (memInfo.Budget - memInfo.CurrentUsage) / (1024 * 1024);
                }
                pAdapter3->Release();
            }
            pFactory->Release();
        }
#endif
    }

}