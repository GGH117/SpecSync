#include "../include/SpecSyncTypes.h"
#include "../include/Telemetry.h"
#include "../include/OptimizationEngine.h"
#include "../include/SettingsRegistry.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

int main() {
    SpecSync::Telemetry telemetry;
    SpecSync::OptimizationEngine optimizer;
    SpecSync::SettingsRegistry registry;

    // Register Handlers
    registry.BindSetting(SpecSync::Setting::ResolutionScale, [](float val) {
        std::cout << "\n[ENGINE] Engine Scaling Resolution to: " << (val * 100) << "%\n";
    });
    
    registry.BindSetting(SpecSync::Setting::TextureStreamingBudget, [](float val) {
        std::cout << "\n[ENGINE] Engine Dropping Texture Budget to: " << (val * 100) << "%\n";
    });

    float deltaTime = 0.1f;
    std::cout << "SpecSync Active (Threaded & Cross-Platform)\n-------------------------------------------\n";

    // Run a 20-second simulated game loop
    for (int i = 0; i < 200; ++i) {
        telemetry.Tick(deltaTime);
        auto data = telemetry.GetLatestData();
        auto commands = optimizer.Process(data, deltaTime);

        if (!commands.empty()) registry.ExecuteCommands(commands);

        std::cout << "\r" 
                  << "CPU: " << std::setw(3) << (int)data.CpuUsage << "% | "
                  << "GPU: " << std::setw(3) << (int)data.GpuUsage << "% | "
                  << "Temp: " << (int)data.GpuTemp << "C | "
                  << "VRAM: " << std::setw(4) << data.AvailableVRAM << "MB | "
                  << "FPS: " << std::setw(3) << (int)data.CurrentFPS << "   ";
        
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "\n\nSimulation Complete. Safe Exit.\n";
    return 0;
}