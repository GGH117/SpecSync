#pragma once
#include <string>
#include <cstdint>

namespace SpecSync 
{
    struct HardwareProfile 
    {
        // Requirements: CPU, GPU, and RAM 
        std::string CPU_Model;
        uint32_t CPU_Cores;
        uint32_t SystemRAM_GB;
        
        std::string GPU_Model;
        uint32_t GPU_VRAM_MB;
        
        bool IsMinimumReqMet() const;
    };

    class HardwareScanner 
    {
    public:
        static HardwareProfile ScanHostSystem();
    };
}