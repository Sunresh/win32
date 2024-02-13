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
    PreferenceManager(){}
    std::string getprefString(const std::string& key);
    bool SetPreference(const std::string& key, const std::string& value);
    bool SetPreferenceW(const std::string& key, const wchar_t* text);
    bool schmittTrigger(double inputValue, double upperThreshold, double lowerThreshold, bool output);
    bool CheckAndCreateFolder(std::string folderName,std::string sub);
private:
    std::string filename;
 

};

class SchmittTrigger {
public:
	SchmittTrigger(double upperThreshold, double lowerThreshold)
		: upperThreshold_(upperThreshold), lowerThreshold_(lowerThreshold), output_(false) {
	}
	bool processInput(double inputValue) {
		if (inputValue >= upperThreshold_) {
			output_ = true;
		}
		else if (inputValue <= lowerThreshold_) {
			output_ = false;
		}
		return output_;
	}
	bool getOutput() const {
		return output_;
	}

private:
	double upperThreshold_;
	double lowerThreshold_;
	bool output_;
};

#endif // PREFERENCE_MANAGER_H
