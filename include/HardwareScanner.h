#pragma once
#include <string>
#include <cstdint>

namespace SpecSync {

    struct HardwareProfile {
        std::string CpuName;
        uint32_t CpuLogicalCores;
        std::string GpuName;
        size_t TotalVRAM_MB;
        size_t TotalSystemRAM_MB;
        bool bIsLegacyGPU;
    };

    class HardwareScanner {
    public:
        static HardwareProfile RunBaselineScan();

    private:
        static void ScanCPU(HardwareProfile& OutProfile);
        static void ScanGPU(HardwareProfile& OutProfile);
        static void ScanRAM(HardwareProfile& OutProfile);
    };

}