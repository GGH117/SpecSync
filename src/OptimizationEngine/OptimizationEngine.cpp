#include "../../include/SpecSyncTypes.h"
#include <vector>

namespace SpecSync {

    class OptimizationEngine {
    private:
        float m_AccumulatedTime = 0.0f;
        const float m_ActionDelay = 2.0f; 
        float m_CurrentScale = 1.0f;

    public:
        // Use the vector of OptimizationCommand defined in SpecSyncTypes.h
        std::vector<OptimizationCommand> Process(const FrameData& data, float dt) {
            std::vector<OptimizationCommand> commands;

            if (data.CurrentFPS < 57.0f && m_CurrentScale > 0.65f) {
                m_AccumulatedTime += dt;
                if (m_AccumulatedTime >= m_ActionDelay) {
                    m_CurrentScale -= 0.05f;
                    commands.push_back({Setting::ResolutionScale, m_CurrentScale});
                    m_AccumulatedTime = 0.0f;
                }
            } 
            else if (data.CurrentFPS > 75.0f && m_CurrentScale < 1.0f) {
                m_AccumulatedTime += dt;
                if (m_AccumulatedTime >= m_ActionDelay) {
                    m_CurrentScale += 0.05f;
                    commands.push_back({Setting::ResolutionScale, m_CurrentScale});
                    m_AccumulatedTime = 0.0f;
                }
            }
            else {
                m_AccumulatedTime = 0.0f;
            }

            return commands;
        }
    };
}