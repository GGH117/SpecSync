#include "../include/Telemetry.h"
#include <fstream>

namespace SpecSync 
{
    // ... StartFrame and EndFrame implementation ...

    void Telemetry::ExportToJson(const TelemetryData& Data) 
    {
        std::ofstream file("telemetry_sync.json");
        if (file.is_open()) {
            file << "{\n";
            file << "  \"fps\": " << Data.FPS << ",\n";
            file << "  \"frame_time\": " << Data.FrameTime_MS << ",\n";
            file << "  \"cpu_temp\": " << Data.CPU_Temp_C << ",\n";
            file << "  \"gpu_temp\": " << Data.GPU_Temp_C << "\n";
            file << "}";
            file.close();
        }
    }

    TelemetryData Telemetry::GetLatestData() {
        TelemetryData Data;
        Data.FrameTime_MS = LastFrameDuration;
        Data.FPS = CalculateCurrentFPS(LastFrameDuration);
        Data.CPU_Temp_C = 65.0f; 
        Data.GPU_Temp_C = 70.0f;
        return Data;
    }
}