#include "../../include/OptimizationEngine.h"

namespace SpecSync {
    std::vector<OptimizationCommand> OptimizationEngine::Process(const FrameData& data, float dt) {
        std::vector<OptimizationCommand> commands;

        // 1. Thermal Guardrail
        if (data.GpuTemp > 85.0f) {
            m_CurrentScale = 0.50f;
            commands.push_back({Setting::ResolutionScale, m_CurrentScale});
            return commands;
        }

        // 2. VRAM Safety
        if (data.AvailableVRAM < 512) {
            commands.push_back({Setting::TextureStreamingBudget, 0.75f});
            return commands;
        }

        // 3. Performance Scaling
        if (data.CurrentFPS < 57.0f && m_CurrentScale > 0.65f) {
            m_AccumulatedTime += dt;
            if (m_AccumulatedTime >= 2.0f) {
                m_CurrentScale -= 0.05f;
                commands.push_back({Setting::ResolutionScale, m_CurrentScale});
                m_AccumulatedTime = 0.0f;
            }
        }
        return commands;
    }
}