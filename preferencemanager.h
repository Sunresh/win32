#ifndef PREFERENCE_MANAGER_H
#define PREFERENCE_MANAGER_H

#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Resource.h"
#include <map>
#include <iostream>
#include <Shlobj.h>
#include <Shlwapi.h>

class PreferenceManager {
public:
    std::string getprefString(const std::string& key);
    void SetPreference(const std::string& key, const std::string& value);
    void SetPreferenceW(const std::string& key, const wchar_t* text);
    bool schmittTrigger(double inputValue, double upperThreshold, double lowerThreshold, bool output);
    bool CheckAndCreateFolder(std::string folderName,std::string sub);
private:
    std::string filename;
 

};

#endif // PREFERENCE_MANAGER_H
