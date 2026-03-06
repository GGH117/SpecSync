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

    // Register Engine Callbacks
    registry.BindSetting(SpecSync::Setting::ResolutionScale, [](float val) {
        std::cout << "\n[ENGINE] Setting r.ScreenPercentage to " << (val * 100) << "\n";
    });

    float deltaTime = 0.1f;
    std::cout << "SpecSync Active: System Architecture - Library Model\n";

    while (true) {
        telemetry.Tick(deltaTime);
        auto data = telemetry.GetLatestData();
        auto commands = optimizer.Process(data, deltaTime);

        if (!commands.empty()) registry.ExecuteCommands(commands);

        std::cout << "\r" 
                  << "CPU: " << std::setw(3) << (int)data.CpuUsage << "% | "
                  << "GPU: " << std::setw(3) << (int)data.GpuUsage << "% | "
                  << "VRAM: " << std::setw(4) << data.AvailableVRAM << "MB | "
                  << "FPS: " << std::setw(3) << (int)data.CurrentFPS << " | "
                  << "TEMP: " << (int)data.GpuTemp << "C   ";
        
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}