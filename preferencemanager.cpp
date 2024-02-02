#include "PreferenceManager.h"


// Function to save preferences to the registry
bool PreferenceManager::SetPreference(const std::string& key, const std::string& value) {
    std::wstring wideKey(key.begin(), key.end());
    std::wstring wideValue(value.begin(), value.end());
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\YourAppName", 0, nullptr, 0, KEY_SET_VALUE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        if (RegSetValueExW(hKey, wideKey.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(wideValue.c_str()), static_cast<DWORD>(sizeof(wchar_t) * (wideValue.length() + 1))) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}
bool PreferenceManager::SetPreferenceW(const std::string& key, const wchar_t* text) {
    std::wstring wideKey(key.begin(), key.end());
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\YourAppName", 0, nullptr, 0, KEY_SET_VALUE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        if (RegSetValueExW(hKey, wideKey.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(text), static_cast<DWORD>((wcslen(text) + 1) * sizeof(wchar_t))) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }
        else {
            return false;
        }
    }
    return false;
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


bool PreferenceManager::CheckAndCreateFolder(std::string folder, std::string subfolder) {

    int size = MultiByteToWideChar(CP_UTF8, 0, folder.c_str(), -1, nullptr, 0);
    int sizer = MultiByteToWideChar(CP_UTF8, 0, subfolder.c_str(), -1, nullptr, 0);

    wchar_t* folderName = new wchar_t[size];
    wchar_t* subfolderName = new wchar_t[sizer];

    MultiByteToWideChar(CP_UTF8, 0, folder.c_str(), -1, folderName, size);
    MultiByteToWideChar(CP_UTF8, 0, subfolder.c_str(), -1, subfolderName, sizer);

    wchar_t desktopPath[MAX_PATH];
    if (SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktopPath) != S_OK) {
        return false;
    }

    wchar_t fullPath[MAX_PATH];
    if (PathCombineW(fullPath, desktopPath, folderName) == NULL) {
        return false;
    }

    CreateDirectoryW(fullPath, NULL);

    wchar_t subfolderPath[MAX_PATH];
    if (PathCombineW(subfolderPath, fullPath, subfolderName) == NULL) {
        return false;
    }

    CreateDirectoryW(subfolderPath, NULL);

    SetPreferenceW(CURRENT_FOLDER, subfolderPath);
    delete[] subfolderName;
    delete[] folderName;

    return true;
}

