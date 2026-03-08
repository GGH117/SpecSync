#pragma once
#include "SpecSyncTypes.h"
#include <vector>

namespace SpecSync {
    class OptimizationEngine {
    public:
        OptimizationEngine() : m_CurrentScale(1.0f), m_AccumulatedTime(0.0f) {}
        
        // This MUST be named 'Process' to match the bridge
        std::vector<OptimizationCommand> Process(const FrameData& data, float dt);

    private:
        float m_CurrentScale;
        float m_AccumulatedTime;
    };
}