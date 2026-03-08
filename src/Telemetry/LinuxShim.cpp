#include "../../include/LinuxShim.h"
#include <fstream>
#include <filesystem>
#include <string>

namespace SpecSync {

LinuxShim::LinuxShim() {
    FindHardwarePaths();
}

void LinuxShim::FindHardwarePaths() {
    // 1. Find Thermal Path (amdgpu for Steam Deck/AMD)
    if (std::filesystem::exists("/sys/class/hwmon")) {
        for (const auto& entry : std::filesystem::directory_iterator("/sys/class/hwmon")) {
            std::ifstream nameFile(entry.path().string() + "/name");
            std::string name;
            if (nameFile >> name && (name == "amdgpu" || name == "radeon")) {
                m_GpuTempPath = entry.path().string() + "/temp1_input";
                break;
            }
        }
    }

    // 2. Find DRM VRAM Paths
    std::string drmPath = "/sys/class/drm/card0/device/";
    if (std::filesystem::exists(drmPath + "mem_info_vram_total")) {
        m_VramTotalPath = drmPath + "mem_info_vram_total";
        m_VramUsedPath = drmPath + "mem_info_vram_used";
    }
}

uint32_t LinuxShim::GetAvailableVRAM() {
    if (m_VramTotalPath.empty() || m_VramUsedPath.empty()) return 2048; // Fallback
    
    unsigned long long total = 0, used = 0;
    std::ifstream fTotal(m_VramTotalPath);
    std::ifstream fUsed(m_VramUsedPath);
    
    if (fTotal >> total && fUsed >> used) {
        return static_cast<uint32_t>((total - used) / 1024 / 1024);
    }
    return 2048;
}

float LinuxShim::GetGpuUsage() {
    std::ifstream fUsage("/sys/class/drm/card0/device/gpu_busy_percent");
    float usage = 0.0f;
    if (fUsage >> usage) return usage;
    return 0.0f;
}

float LinuxShim::GetGpuTemp() {
    if (m_GpuTempPath.empty()) return 45.0f; // Default safe idle
    
    std::ifstream tempFile(m_GpuTempPath);
    float milliCelsius = 0.0f;
    if (tempFile >> milliCelsius) return milliCelsius / 1000.0f;
    return 45.0f;
}

float LinuxShim::GetCpuUsage() {
    std::ifstream file("/proc/stat");
    std::string cpu;
    unsigned long long user, nice, system, idle;
    if (file >> cpu >> user >> nice >> system >> idle) {
        unsigned long long total = user + nice + system + idle;
        unsigned long long totalDiff = total - (m_LastTotalUser + m_LastTotalUserLow + m_LastTotalSys + m_LastTotalIdle);
        unsigned long long idleDiff = idle - m_LastTotalIdle;

        m_LastTotalUser = user; m_LastTotalUserLow = nice; m_LastTotalSys = system; m_LastTotalIdle = idle;
        
        if (totalDiff > 0) {
            return (1.0f - (static_cast<float>(idleDiff) / totalDiff)) * 100.0f;
        }
    }
    return 0.0f;
}

} // namespace SpecSync