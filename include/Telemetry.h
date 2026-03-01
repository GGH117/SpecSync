#pragma once
#include <cstdint>
#include <vector>

namespace SpecSync {

    struct FrameData {
        float DeltaTime;       // Time between current and last frame (in ms)
        float CurrentFPS;      
        float GpuUsage;        // 0.0 to 100.0
        float CpuUsage;        // 0.0 to 100.0
        size_t AvailableVRAM;  // Remaining VRAM (in MB)
    };

    class Telemetry {
    public:
        Telemetry();
        ~Telemetry();

        // Called every frame by the game engine
        void Tick(float DeltaTime);

        // Accessors for the Decision Engine
        FrameData GetLatestData() const { return CurrentFrame; }
        float GetAverageFPS(int WindowSize = 60) const;

    private:
        FrameData CurrentFrame;
        std::vector<float> FpsHistory;
        
        // Internal timers to ensure that hardware queries are not performed every frame for performance reasons
        float TimeSinceLastGpuQuery;
        const float GpuQueryInterval = 0.5f; // Query hardware every 500ms

        void UpdateHardwareStats();
    };

}