#include "PreferenceManager.h"


// Function to save preferences to the registry
void PreferenceManager::SetPreference(const std::string& key, const std::string& value) {
    std::wstring wideKey(key.begin(), key.end());
    std::wstring wideValue(value.begin(), value.end());

    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\YourAppName", 0, nullptr, 0, KEY_SET_VALUE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, wideKey.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(wideValue.c_str()), static_cast<DWORD>(sizeof(wchar_t) * (wideValue.length() + 1)));
        RegCloseKey(hKey);
    }
}
void PreferenceManager::SetPreferenceW(const std::string& key, const wchar_t* text) {
    std::wstring wideKey(key.begin(), key.end());
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\YourAppName", 0, nullptr, 0, KEY_SET_VALUE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, wideKey.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(text), static_cast<DWORD>((wcslen(text) + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
    }
}
std::string PreferenceManager::getprefString(const std::string& key) {
    std::wstring wideKey(key.begin(), key.end());
    std::wstring value;
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\YourAppName", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD bufferSize = 1024; // Adjust this size based on your needs
        wchar_t buffer[1024];
        if (RegQueryValueExW(hKey, wideKey.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer), &bufferSize) == ERROR_SUCCESS) {
            value = buffer;
        }
        RegCloseKey(hKey);
    }

    return std::string(value.begin(), value.end());
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