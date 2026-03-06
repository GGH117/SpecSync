#include "../../include/LinuxShim.h"
#ifndef _WIN32
#include <fstream>
#include <filesystem>

namespace SpecSync {

LinuxShim::LinuxShim() {
    FindHardwarePaths();
}

void LinuxShim::FindHardwarePaths() {
    for (const auto& entry : std::filesystem::directory_iterator("/sys/class/hwmon")) {
        std::ifstream nameFile(entry.path().string() + "/name");
        std::string name;
        if (nameFile >> name && name == "amdgpu") {
            m_GpuTempPath = entry.path().string() + "/temp1_input";
            break;
        }
    }

    std::string drmPath = "/sys/class/drm/card0/device/";
    if (std::filesystem::exists(drmPath + "mem_info_vram_total")) {
        m_VramTotalPath = drmPath + "mem_info_vram_total";
        m_VramUsedPath = drmPath + "mem_info_vram_used";
    }
}

uint32_t LinuxShim::GetAvailableVRAM() {
    if (m_VramTotalPath.empty() || m_VramUsedPath.empty()) return 2048; 
    unsigned long long total = 0, used = 0;
    std::ifstream(m_VramTotalPath) >> total;
    std::ifstream(m_VramUsedPath) >> used;
    return static_cast<uint32_t>((total - used) / 1024 / 1024);
}

float LinuxShim::GetGpuUsage() {
    float usage = 0.0f;
    std::ifstream("/sys/class/drm/card0/device/gpu_busy_percent") >> usage;
    return usage;
}

float LinuxShim::GetGpuTemp() {
    if (m_GpuTempPath.empty()) return 0.0f;
    float milliCelsius = 0.0f;
    std::ifstream(m_GpuTempPath) >> milliCelsius;
    return milliCelsius / 1000.0f;
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
        if (totalDiff > 0) return (1.0f - ((float)idleDiff / totalDiff)) * 100.0f;
    }
    return 0.0f;
}

} // namespace SpecSync
#endif