#ifndef SPECSYNC_TYPES_H
#define SPECSYNC_TYPES_H

#include <cstdint>

namespace SpecSync {

    /**
     * @brief Raw hardware performance snapshot.
     * This is the "contract" between the Game Engine and SpecSync.
     */
    struct FrameData {
        float DeltaTime;       // Time since last frame (seconds)
        float CurrentFPS;
        float GpuUsage;        // 0.0 to 100.0
        float CpuUsage;        // 0.0 to 100.0
        float GpuTemp;
        uint64_t AvailableVRAM; // Available video memory (in MB)
    };

    /**
     * @brief Engine settings that SpecSync can manipulate.
     */
    enum class Setting : uint8_t {
        ResolutionScale,
        ShadowQuality,
        TextureStreamingBudget,
        VolumetricFog,
        LODDistance
    };

    /**
     * @brief A single optimization command to be applied by the game.
     */
    struct OptimizationCommand {
        Setting TargetSetting;
        float TargetValue;
    };

} 

#endif // SPECSYNC_TYPES_H