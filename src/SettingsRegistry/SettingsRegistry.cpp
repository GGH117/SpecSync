#include "../../include/SettingsRegistry.h"
#include <iostream>

namespace SpecSync {
    void SettingsRegistry::BindSetting(Setting target, SettingCallback callback) {
        m_Callbacks[target] = callback;
    }

    void SettingsRegistry::ExecuteCommands(const std::vector<OptimizationCommand>& commands) {
        for (const auto& cmd : commands) {
            auto it = m_Callbacks.find(cmd.TargetSetting);
            if (it != m_Callbacks.end()) {
                it->second(cmd.TargetValue);
            } else {
                std::cerr << "[SpecSync Warning] Tried to execute command for unbound setting.\n";
            }
        }
    }
}
