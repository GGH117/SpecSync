#include "../../include/SettingsRegistry.h"
#include <iostream> // For fallback logging

namespace SpecSync {

    void SettingsRegistry::BindSetting(Setting target, SettingCallback callback) {
        m_Callbacks[target] = callback;
    }

    void SettingsRegistry::ExecuteCommands(const std::vector<OptimizationCommand>& commands) {
        for (const auto& cmd : commands) {
            // Check if the developer actually bound a function for this setting
            auto it = m_Callbacks.find(cmd.TargetSetting);
            if (it != m_Callbacks.end()) {
                // Execute the game engine's callback with the new value
                it->second(cmd.TargetValue);
            } else {
                // Fallback warning if SpecSync tries to optimize something the game doesn't support
                std::cerr << "[SpecSync Warning] Tried to execute command for an unbound setting.\n";
            }
        }
    }

}