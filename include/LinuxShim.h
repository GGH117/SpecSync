#pragma once
#include <string>

namespace SpecSync {
    class LinuxShim {
    public:
        LinuxShim();
        float GetGpuTemp();
        float GetCpuUsage();
        
        // New VRAM methods for Linux/Steam Deck
        uint32_t GetAvailableVRAM();
        float GetGpuUsage();

    private:
        std::string m_GpuTempPath;
        std::string m_VramTotalPath;
        std::string m_VramUsedPath;
        
        // CPU tracking variables
        unsigned long long m_LastTotalUser = 0, m_LastTotalUserLow = 0, 
                           m_LastTotalSys = 0, m_LastTotalIdle = 0;
        
        void FindHardwarePaths();
    };
} // namespace SpecSync