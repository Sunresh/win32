#ifndef PREFERENCE_MANAGER_H
#define PREFERENCE_MANAGER_H

#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>  // Include necessary headers
#include <map>
#include <iostream>
#include <Shlobj.h>

class PreferenceManager {
public:
    PreferenceManager() {};
    std::string getprefString(const std::string& key);
    void SetPreference(const std::string& key, const std::string& value);
    void SetPreferenceW(const std::string& key, const wchar_t* text);
    bool schmittTrigger(double inputValue, double upperThreshold, double lowerThreshold, bool output);

private:
    std::string filename;
 

};

#endif // PREFERENCE_MANAGER_H
