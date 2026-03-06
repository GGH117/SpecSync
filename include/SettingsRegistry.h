#pragma once
#include "SpecSyncTypes.h"
#include <functional>
#include <unordered_map>
#include <vector>

namespace SpecSync {
    using SettingCallback = std::function<void(float)>;

    class SettingsRegistry {
    public:
        void BindSetting(Setting target, SettingCallback callback);
        void ExecuteCommands(const std::vector<OptimizationCommand>& commands);

    private:
        std::unordered_map<Setting, SettingCallback> m_Callbacks;
    };
}