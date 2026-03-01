#include "../../include/Telemetry.h"
#include <string>

namespace SpecSync {

    struct OptimizationAction {
        std::string TargetSetting;
        float AdjustmentValue;
        std::string MessageForChatbot;
    };

    class OptimizationEngine {
    public:
        OptimizationAction Analyze(const FrameData& data) {
            // Everything is fine
            OptimizationAction action = {"NONE", 1.0f, "System performance is optimal."};

            // If VRAM is critical under 500MB
            if (data.AvailableVRAM < 500) {
                action = {"TextureQuality", 0.5f, "VRAM is nearly full. I've optimized texture streaming."};
            }
            // If FPS is dipping below a target
            else if (data.CurrentFPS < 55.0f) {
                action = {"ResolutionScale", 0.85f, "FPS dropped. I'm scaling resolution to keep things smooth."};
            }

            return action;
        }
    };
}