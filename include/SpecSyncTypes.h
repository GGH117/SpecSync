#pragma once
#include <cstdint>

namespace SpecSync {
    struct FrameData {
        float DeltaTime;
        float CurrentFPS;
        float CpuUsage;
        float GpuUsage;
        float GpuTemp;       
        uint32_t AvailableVRAM;
    };

    enum class Setting {
        ResolutionScale,
        TextureStreamingBudget,
        ShadowQuality
    };

    struct OptimizationCommand {
        Setting TargetSetting;
        float TargetValue;
    };
}