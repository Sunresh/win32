#ifndef PREFERENCE_MANAGER_H
#define PREFERENCE_MANAGER_H

#include <windows.h>
#include <string>
#include "json.hpp"  // Include json.hpp where the json type is used
#include <sstream>
#include <fstream>
#include <iostream>  // Include necessary headers
#include <map>

class PreferenceManager {
public:
    PreferenceManager(const std::string& filename = "preferences.json");

    void LoadPreferences();
    void SavePreferences();
    std::string GetPreference(const std::string& key, const std::string& defaultValue = "");
    void SetPreference(const std::string& key, const std::string& value);
    bool schmittTrigger(double inputValue, double upperThreshold, double lowerThreshold, bool output);

private:
    std::string filename;
    nlohmann::json preferences;  // Use nlohmann::json as a member variable
};

#endif // PREFERENCE_MANAGER_H
