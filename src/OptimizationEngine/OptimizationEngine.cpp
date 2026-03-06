#include "../../include/OptimizationEngine.h"

namespace SpecSync {
    std::vector<OptimizationCommand> OptimizationEngine::Process(const FrameData& data, float dt) {
        std::vector<OptimizationCommand> commands;

        // --- 0. EMERGENCY: THERMAL GUARDRAIL ---
        if (data.GpuTemp > 85.0f) {
            m_CurrentScale = 0.50f; 
            commands.push_back({Setting::ResolutionScale, m_CurrentScale});
            return commands; 
        }

        // --- 1. PRIORITY: VRAM SAFETY ---
        if (data.AvailableVRAM < 500) { 
            m_AccumulatedTime += dt;
            if (m_AccumulatedTime >= m_ActionDelay) {
                commands.push_back({Setting::TextureStreamingBudget, 0.75f}); 
                m_AccumulatedTime = 0.0f;
            }
            return commands; 
        }

        // --- 2. SECONDARY: FPS PERFORMANCE ---
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
}