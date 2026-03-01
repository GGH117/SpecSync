#pragma once
#include "SpecSyncTypes.h" // Include the light-weight types
#include <vector>

namespace SpecSync {
    class Telemetry {
    public:
        void Tick(float dt);
        FrameData GetLatestData() const;
    private:
        FrameData m_CurrentFrame;
        // ... internal logic hidden in .cpp
    };
}