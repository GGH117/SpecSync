#pragma once
#include <chrono>

namespace SpecSync 
{
    struct TelemetryData 
    {
        double FrameTime_MS;
        float FPS;
        float CPU_Temp_C;
        float GPU_Temp_C;
    };

    class Telemetry
    {
    public:
        void StartFrame();
        void EndFrame();
        
        // ADD THIS LINE BELOW:
        void ExportToJson(const TelemetryData& Data);
        
        TelemetryData GetLatestData();

    private:
        std::chrono::steady_clock::time_point FrameStart;
        double LastFrameDuration;
        float CalculateCurrentFPS(double frameTimeMS);
    };
}