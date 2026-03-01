#pragma once
#include "SpecSyncTypes.h"
#include <vector>

namespace SpecSync {
    class Telemetry {
    public:
        Telemetry();  // Constructor
        ~Telemetry(); // Destructor

        void Tick(float dt);
        FrameData GetLatestData() const;

    private:
        // Internal Helpers
        float GetAverageFPS() const;
        void UpdateHardwareStats();

        // Data Storage
        FrameData CurrentFrame;
        std::vector<float> FpsHistory;
        
        // Timers
        float TimeSinceLastGpuQuery;
        const float GpuQueryInterval = 1.0f; // Query hardware once per second
    };
}