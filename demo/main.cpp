#include "../include/SpecSyncTypes.h"
#include "../include/Telemetry.h"
#include "../include/OptimizationEngine.h"
#include "../include/SettingsRegistry.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

int main() {
    std::cout << "==========================================\n";
    std::cout << "   SpecSync Full System Integration Test  \n";
    std::cout << "==========================================\n\n";

    // 1. Initialize the Framework Modules
    SpecSync::Telemetry telemetry;
    SpecSync::OptimizationEngine optimizer;
    SpecSync::SettingsRegistry registry;

    // 2. The Game Engine "Binds" its internal commands to SpecSync
    // (Simulating Unreal Engine console commands here)
    registry.BindSetting(SpecSync::Setting::ResolutionScale, [](float newValue) {
        std::cout << "\n   >>> [UNREAL ENGINE] Executing: r.ScreenPercentage " 
                  << std::fixed << std::setprecision(0) << (newValue * 100.0f) << " <<<\n";
    });

    registry.BindSetting(SpecSync::Setting::TextureStreamingBudget, [](float newValue) {
        std::cout << "\n   >>> [UNREAL ENGINE] Executing: r.Streaming.PoolSize Multiplier " 
                  << std::fixed << std::setprecision(2) << newValue << " <<<\n";
    });

    float deltaTime = 0.1f; 

    std::cout << "Monitoring Hardware (Press Ctrl+C to stop)...\n";
    std::cout << "---------------------------------------------\n";

    // 3. The Main Game Loop
    for (int i = 0; i < 100; ++i) { // Running for 10 seconds (100 ticks)
        
        // A. Eyes: Look at the hardware
        telemetry.Tick(deltaTime);
        auto data = telemetry.GetLatestData();

        // B. Brain: Decide if we need to act
        auto commands = optimizer.Process(data, deltaTime);

        // C. Hands: Execute the actions via the Registry
        if (!commands.empty()) {
            registry.ExecuteCommands(commands);
        }

        // Print the dashboard overlay
        std::cout << "\r" 
                  << "CPU: " << std::setw(3) << (int)data.CpuUsage << "% | "
                  << "GPU: " << std::setw(3) << (int)data.GpuUsage << "% | "
                  << "VRAM: " << std::setw(4) << data.AvailableVRAM << "MB | "
                  << "FPS: " << std::setw(3) << (int)data.CurrentFPS << " | "
                  << (commands.empty() ? "Status: OK   " : "ACTION FIRED!");
        
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\n\nSimulation Complete.\n";
    return 0;
}