#ifndef SCREEN_RECORD_H
#define SCREEN_RECORD_H
#include <Windows.h>
#include "preferencemanager.h"

class ScreenRecord {
public:
	PreferenceManager pref;
	void CaptureAndSaveScreenshot(const char* fileName);
};

#endif