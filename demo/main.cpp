#include "../include/SpecSyncTypes.h"
#include "../include/Telemetry.h"
#include "../include/OptimizationEngine.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

int main() {
    std::cout << "==========================================\n";
    std::cout << "   SpecSync Real-Time Hardware Monitor    \n";
    std::cout << "==========================================\n\n";

    SpecSync::Telemetry telemetry;
    SpecSync::OptimizationEngine optimizer;

    // Simulates a 10-second run
    auto startTime = std::chrono::steady_clock::now();
    float simulateDuration = 10.0f; 
    float deltaTime = 0.1f; // 100ms updates

    while (true) {
        // Update Telemetry (Queries DXGI 1.4)
        telemetry.Tick(deltaTime);
        SpecSync::FrameData data = telemetry.GetLatestData();

        // Process Optimization Logic
        auto commands = optimizer.Process(data, deltaTime);

        // Print Real Stats to Console
        // Change the print line in your loop to this:
std::cout << "\r" 
          << "CPU: " << std::setw(3) << (int)data.CpuUsage << "% | "
          << "GPU: " << std::setw(3) << (int)data.GpuUsage << "% | "
          << "VRAM: " << std::setw(4) << data.AvailableVRAM << "MB | "
          << "FPS: " << std::setw(3) << (int)data.CurrentFPS << " | ";

        if (!commands.empty()) {
            std::cout << ">> ACTION TRIGGERED! <<";
        } else {
            std::cout << "Status: Stable        ";
        }

        // Flush output so the \r trick works
        std::cout.flush();

        // Check if 10 seconds have passed
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        if (std::chrono::duration<float>(elapsed).count() > simulateDuration) break;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\n\nMonitoring Complete.\n";
    return 0;
}