#ifndef MY_UI_H
#define MY_UI_H
#include <Windows.h>
#include "Resource.h"
#include <iostream>

class MyUI {
public:
	HWND hCombo, hWndHeight, hwndPP, hFrame, g_hFrame1, zoomfram, graphframe, pztGraphframe;
	static HWND CreateButton(const wchar_t* text, int x, int y, int buttonWidth, int buttonHeight, HWND parent, int id, DWORD style = BS_PUSHBUTTON);
	static HWND CreateStaticText(const wchar_t* text, int x, int y, int width, int height, HWND parent, int id, DWORD style = WS_BORDER);
	HWND mainUi(HWND parent);
	HWND GetCamHandle() const;
	HWND GetZoomCamHandle() const;
	HWND GetBDgraphHandle() const;
	HWND GetPZTgraphHandle() const;
};


#endif // !MY_UI_H
