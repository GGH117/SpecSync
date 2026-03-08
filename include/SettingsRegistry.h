#pragma once
#include "SpecSyncTypes.h"
#include <map>

namespace SpecSync {
    class SettingsRegistry {
    public:
        void UpdateSetting(Setting setting, float value);
        float GetSetting(Setting setting);
    private:
        std::map<Setting, float> m_Settings;
    };
}