#include "../../include/OptimizationEngine.h"

namespace SpecSync {

    std::vector<OptimizationCommand> OptimizationEngine::Process(const FrameData& data, float dt) {
        std::vector<OptimizationCommand> commands;

        // PRIORITY: VRAM SAFETY
        // If VRAM is critical (< 500MB), throttle textures and ignore FPS logic
        if (data.AvailableVRAM < 500) { 
            m_AccumulatedTime += dt;
            if (m_AccumulatedTime >= m_ActionDelay) {
                commands.push_back({Setting::TextureStreamingBudget, 0.75f}); 
                m_AccumulatedTime = 0.0f;
            }
            return commands; 
        }

        // SECONDARY: FPS PERFORMANCE
        // DOWN-SHIFT: FPS is struggling
        if (data.CurrentFPS < 57.0f && m_CurrentScale > 0.65f) {
            m_AccumulatedTime += dt;
            if (m_AccumulatedTime >= m_ActionDelay) {
                m_CurrentScale -= 0.05f; 
                commands.push_back({Setting::ResolutionScale, m_CurrentScale});
                m_AccumulatedTime = 0.0f;
            }
        } 
        // UP-SHIFT: Massive performance headroom
        else if (data.CurrentFPS > 75.0f && m_CurrentScale < 1.0f) {
            m_AccumulatedTime += dt;
            if (m_AccumulatedTime >= m_ActionDelay) {
                m_CurrentScale += 0.05f;
                commands.push_back({Setting::ResolutionScale, m_CurrentScale});
                m_AccumulatedTime = 0.0f;
            }
        } 
        // STABLE: Reset timer
        else {
            m_AccumulatedTime = 0.0f;
        }

        return commands;
    }

} // namespace SpecSync