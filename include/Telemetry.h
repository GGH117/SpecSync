#pragma once
#include "SpecSyncTypes.h"
#include <vector>
#include <thread>
#include <atomic>

namespace SpecSync {
    class Telemetry {
    public:
        Telemetry();
        ~Telemetry();

        void Tick(float dt);
        FrameData GetLatestData() const;

    private:
        void UpdateHardwareStats();
        void BackgroundThreadLoop();
        float GetAverageFPS() const;

        FrameData CurrentFrame;
        std::vector<float> FpsHistory;
        
        std::atomic<bool> m_IsRunning;
        std::thread m_WorkerThread;
    };
}