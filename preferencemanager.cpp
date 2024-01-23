#include "PreferenceManager.h"

PreferenceManager::PreferenceManager(const std::string& filename) : filename(filename) {
    PWSTR desktopPath;
    if (SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath) == S_OK) {
        // Combine the desktop path with the folder name
        std::wstring folderPath = std::wstring(desktopPath) + L"\\MyNewFolder";

        // Create the folder
        if (CreateDirectoryW(folderPath.c_str(), NULL) != 0) {
            std::wcout << L"Folder created successfully: " << folderPath << std::endl;
        }
        else {
            std::cerr << "Failed to create folder. Error code: " << GetLastError() << std::endl;
        }

        // Free the memory allocated by SHGetKnownFolderPath
        CoTaskMemFree(desktopPath);
    }
    else {
        std::cerr << "Failed to retrieve desktop folder path." << std::endl;
    }
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

// Function to save preferences to the registry
void SavePreferences(const wchar_t* keyName, const wchar_t* valueName, DWORD value) {
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, keyName, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, valueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&value), sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

// Function to load preferences from the registry
DWORD LoadPreferences(const wchar_t* keyName, const wchar_t* valueName, DWORD defaultValue) {
    DWORD value = defaultValue;
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, keyName, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dataSize = sizeof(DWORD);
        RegQueryValueExW(hKey, valueName, NULL, NULL, reinterpret_cast<BYTE*>(&value), &dataSize);
        RegCloseKey(hKey);
    }
    return value;
}

bool PreferenceManager::schmittTrigger(double inputValue, double upperThreshold, double lowerThreshold, bool output = false) {
    if (inputValue >= upperThreshold) {
        output = true;
    }
    else if (inputValue <= lowerThreshold) {
        output = false;
    }
    return output;
}