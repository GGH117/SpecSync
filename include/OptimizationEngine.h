#pragma once
#include "SpecSyncTypes.h"
#include <vector>

namespace SpecSync {
    class OptimizationEngine {
    public:
        std::vector<OptimizationCommand> Process(const FrameData& data, float dt);

    private:
        float m_CurrentScale = 1.0f;
        float m_AccumulatedTime = 0.0f;
        const float m_ActionDelay = 2.0f; 
    };
}