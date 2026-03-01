#include "../../include/Telemetry.h"
#include <dxgi1_4.h>
#include <wrl.h> 
#include <pdh.h>
#include <numeric>

using Microsoft::WRL::ComPtr;

// Static handles for the CPU query
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

namespace SpecSync {

    Telemetry::Telemetry() : TimeSinceLastGpuQuery(0.0f) {
        CurrentFrame = {0.0f, 0.0f, 0.0f, 0.0f, 0};
        
        // Initialize PDH for CPU tracking
        PdhOpenQuery(NULL, NULL, &cpuQuery);
        PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
    }

    Telemetry::~Telemetry() {
        PdhCloseQuery(cpuQuery);
    }

    void Telemetry::Tick(float dt) {
        CurrentFrame.DeltaTime = dt;
        if (dt > 0.0f) {
            float currentFps = 1.0f / dt;
            FpsHistory.push_back(currentFps);
            if (FpsHistory.size() > 60) FpsHistory.erase(FpsHistory.begin());
            CurrentFrame.CurrentFPS = GetAverageFPS();
        }

        TimeSinceLastGpuQuery += dt;
        if (TimeSinceLastGpuQuery >= GpuQueryInterval) {
            UpdateHardwareStats();
            TimeSinceLastGpuQuery = 0.0f;
        }
    }

    void Telemetry::UpdateHardwareStats() {
        // GPU Stats via DXGI
        ComPtr<IDXGIFactory4> factory;
        if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
            ComPtr<IDXGIAdapter1> adapter;
            if (SUCCEEDED(factory->EnumAdapters1(0, &adapter))) {
                ComPtr<IDXGIAdapter3> adapter3;
                if (SUCCEEDED(adapter.As(&adapter3))) {
                    DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
                    if (SUCCEEDED(adapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo))) {
                        CurrentFrame.AvailableVRAM = (memInfo.Budget - memInfo.CurrentUsage) / 1024 / 1024;
                        CurrentFrame.GpuUsage = ((float)memInfo.CurrentUsage / (float)memInfo.Budget) * 100.0f;
                    }
                }
            }
        }

        // CPU Stats via PDH
        PDH_FMT_COUNTERVALUE counterVal;
        PdhCollectQueryData(cpuQuery);
        PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
        CurrentFrame.CpuUsage = (float)counterVal.doubleValue;
    }

    float Telemetry::GetAverageFPS() const {
        if (FpsHistory.empty()) return 0.0f;
        float sum = std::accumulate(FpsHistory.begin(), FpsHistory.end(), 0.0f);
        return sum / FpsHistory.size();
    }

    FrameData Telemetry::GetLatestData() const { return CurrentFrame; }
}