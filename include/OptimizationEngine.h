#pragma once
#include "SpecSyncTypes.h"
#include <vector>

namespace SpecSync {
    class OptimizationEngine {
    private:
        float m_AccumulatedTime = 0.0f;
        const float m_ActionDelay = 2.0f; 
        float m_CurrentScale = 1.0f;

    public:
        // Returns a list of optimization commands to apply this frame
        std::vector<OptimizationCommand> Process(const FrameData& data, float dt);
    };
}