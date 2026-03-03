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
    // Order: DeltaTime, CurrentFPS, CpuUsage, GpuUsage, GpuTemp, AvailableVRAM
    CurrentFrame = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0};
    
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
    // --- 1. GPU Memory Logic (DXGI) ---
    ComPtr<IDXGIFactory4> factory;
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        // ... (Your existing adapter and memInfo code) ...
        if (SUCCEEDED(adapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo))) {
            CurrentFrame.AvailableVRAM = (memInfo.Budget - memInfo.CurrentUsage) / 1024 / 1024;
            CurrentFrame.GpuUsage = ((float)memInfo.CurrentUsage / (float)memInfo.Budget) * 100.0f;
        }
    }

    // --- 2. CPU Usage Logic (PDH) ---
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    CurrentFrame.CpuUsage = (float)counterVal.doubleValue;

    // --- 3. THERMAL DATA (MOCK) ---
    // PLACE IT HERE: This ensures every time telemetry updates, it has a temp value.
    CurrentFrame.GpuTemp = 60.0f; 
}

    float Telemetry::GetAverageFPS() const {
        if (FpsHistory.empty()) return 0.0f;
        float sum = std::accumulate(FpsHistory.begin(), FpsHistory.end(), 0.0f);
        return sum / FpsHistory.size();
    }

    FrameData Telemetry::GetLatestData() const { return CurrentFrame; }
}