#include "PreferenceManager.h"

PreferenceManager::PreferenceManager(const std::string& filename) : filename(filename) {
    LoadPreferences();
}

void PreferenceManager::LoadPreferences() {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> preferences;
        file.close();
    }
}

void PreferenceManager::SavePreferences() {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << preferences;
        file.close();
    }
}

std::string PreferenceManager::GetPreference(const std::string& key, const std::string& defaultValue) {
    // Check if the preferences object has the specified key
    if (preferences.is_object() && preferences.find(key) != preferences.end()) {
        // If the key is found, return its value
        return preferences[key].get<std::string>();
    }
    else {
        // If the key is not found or the preferences object is not an object,
        // return the default value
        return defaultValue;
    }
}

void PreferenceManager::SetPreference(const std::string& key, const std::string& value) {
    preferences[key] = value;
    SavePreferences();
}
