#pragma once
#include <cstdint>

namespace SpecSync {
    struct FrameData {
        float DeltaTime = 0.0f;
        float CurrentFPS = 0.0f;
        float CpuUsage = 0.0f;
        float GpuUsage = 0.0f;
        float GpuTemp = 0.0f;       
        uint32_t AvailableVRAM = 0;
    };

    enum class Setting { ResolutionScale, TextureStreamingBudget, ShadowQuality };

    struct OptimizationCommand {
        Setting TargetSetting;
        float TargetValue;
    };
}