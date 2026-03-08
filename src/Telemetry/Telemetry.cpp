#include "../../include/Telemetry.h"
#include <numeric>

#ifdef PLATFORM_WINDOWS
    #include <dxgi1_4.h>
    #include <pdh.h>
    #include <wrl.h>
    using Microsoft::WRL::ComPtr;

    // Handles moved to top level to fix "undeclared identifier"
    static PDH_HQUERY cpuQuery;
    static PDH_HCOUNTER cpuTotal;
#else
    #include "../../include/LinuxShim.h"
    static SpecSync::LinuxShim g_LinuxShim;
#endif

namespace SpecSync {

Telemetry::Telemetry() : m_IsRunning(true) {
    CurrentFrame = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0};
    
#ifdef PLATFORM_WINDOWS
    PdhOpenQuery(NULL, NULL, &cpuQuery);
    PdhAddEnglishCounterW(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
#endif
    m_WorkerThread = std::thread(&Telemetry::BackgroundThreadLoop, this);
}

Telemetry::~Telemetry() {
    m_IsRunning = false;
    if (m_WorkerThread.joinable()) m_WorkerThread.join();
#ifdef PLATFORM_WINDOWS
    PdhCloseQuery(cpuQuery);
#endif
}

void Telemetry::BackgroundThreadLoop() {
    while (m_IsRunning) {
        UpdateHardwareStats();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Telemetry::Tick(float dt) {
    CurrentFrame.DeltaTime = dt;
    if (dt > 0.0f) {
        FpsHistory.push_back(1.0f / dt);
        if (FpsHistory.size() > 60) FpsHistory.erase(FpsHistory.begin());
        CurrentFrame.CurrentFPS = GetAverageFPS();
    }
}

float Telemetry::GetAverageFPS() const {
    if (FpsHistory.empty()) return 0.0f;
    return std::accumulate(FpsHistory.begin(), FpsHistory.end(), 0.0f) / (float)FpsHistory.size();
}

void Telemetry::UpdateHardwareStats() {
#ifdef PLATFORM_WINDOWS
    ComPtr<IDXGIFactory4> factory;
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        ComPtr<IDXGIAdapter1> adapter;
        if (SUCCEEDED(factory->EnumAdapters1(0, &adapter))) {
            ComPtr<IDXGIAdapter3> adapter3;
            if (SUCCEEDED(adapter.As(&adapter3))) {
                DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
                if (SUCCEEDED(adapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo))) {
                    CurrentFrame.AvailableVRAM = (uint32_t)((memInfo.Budget - memInfo.CurrentUsage) / 1024 / 1024);
                    CurrentFrame.GpuUsage = ((float)memInfo.CurrentUsage / (float)memInfo.Budget) * 100.0f;
                }
            }
        }
    }
    PDH_FMT_COUNTERVALUE cv;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &cv);
    CurrentFrame.CpuUsage = (float)cv.doubleValue;
    CurrentFrame.GpuTemp = 60.0f; 
#else
    CurrentFrame.GpuTemp = g_LinuxShim.GetGpuTemp();
    CurrentFrame.CpuUsage = g_LinuxShim.GetCpuUsage();
    CurrentFrame.GpuUsage = g_LinuxShim.GetGpuUsage();
    CurrentFrame.AvailableVRAM = g_LinuxShim.GetAvailableVRAM();
#endif
}

FrameData Telemetry::GetLatestData() const { return CurrentFrame; }

} // namespace SpecSync