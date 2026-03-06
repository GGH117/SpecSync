#include "../../include/Telemetry.h"
#include <numeric>

#ifdef _WIN32
    #include <dxgi1_4.h>
    #include <pdh.h>
    #include <wrl.h>
    using Microsoft::WRL::ComPtr;
    static PDH_HQUERY cpuQuery;
    static PDH_HCOUNTER cpuTotal;
#else
    #include "../../include/LinuxShim.h"
    static SpecSync::LinuxShim g_LinuxShim;
#endif

namespace SpecSync {

    Telemetry::Telemetry() : m_IsRunning(true) {
        CurrentFrame = {0.0f, 0.0f, 0.0f, 0.0f, 60.0f, 0};
        
#ifdef _WIN32
        PdhOpenQuery(NULL, NULL, &cpuQuery);
        PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
#endif

        m_WorkerThread = std::thread(&Telemetry::BackgroundThreadLoop, this);
    }

    Telemetry::~Telemetry() {
        m_IsRunning = false;
        if (m_WorkerThread.joinable()) {
            m_WorkerThread.join();
        }
#ifdef _WIN32
        PdhCloseQuery(cpuQuery);
#endif
    }

    void Telemetry::BackgroundThreadLoop() {
        while (m_IsRunning) {
            UpdateHardwareStats();
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Non-blocking 10Hz poll
        }
    }

    void Telemetry::Tick(float dt) {
        CurrentFrame.DeltaTime = dt;
        if (dt > 0.0f) {
            float currentFps = 1.0f / dt;
            FpsHistory.push_back(currentFps);
            if (FpsHistory.size() > 60) FpsHistory.erase(FpsHistory.begin());
            CurrentFrame.CurrentFPS = GetAverageFPS();
        }
    }

    void Telemetry::UpdateHardwareStats() {
#ifdef _WIN32
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
        PDH_FMT_COUNTERVALUE counterVal;
        PdhCollectQueryData(cpuQuery);
        PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
        CurrentFrame.CpuUsage = (float)counterVal.doubleValue;
        CurrentFrame.GpuTemp = 60.0f; // Windows Mock Temp
#else
        CurrentFrame.GpuTemp = g_LinuxShim.GetGpuTemp();
        CurrentFrame.CpuUsage = g_LinuxShim.GetCpuUsage();
        CurrentFrame.GpuUsage = g_LinuxShim.GetGpuUsage();
        CurrentFrame.AvailableVRAM = g_LinuxShim.GetAvailableVRAM();
#endif
    }

    float Telemetry::GetAverageFPS() const {
        if (FpsHistory.empty()) return 0.0f;
        return std::accumulate(FpsHistory.begin(), FpsHistory.end(), 0.0f) / FpsHistory.size();
    }

    FrameData Telemetry::GetLatestData() const { return CurrentFrame; }

} // namespace SpecSync