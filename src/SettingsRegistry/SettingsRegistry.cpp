#include "../../include/SettingsRegistry.h"

namespace SpecSync {
    
    void SettingsRegistry::UpdateSetting(Setting setting, float value) {
        m_Settings[setting] = value;
    }

    float SettingsRegistry::GetSetting(Setting setting) {
        auto it = m_Settings.find(setting);
        if (it != m_Settings.end()) {
            return it->second;
        }
        return 0.0f; // Default fallback if the setting hasn't been written yet
    }

} // namespace SpecSync