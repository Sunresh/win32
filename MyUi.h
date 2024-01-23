#ifndef MY_UI_H
#define MY_UI_H
#include <Windows.h>
#include "Resource.h"
#include <iostream>
#include <CommCtrl.h>

class MyUI {
public:
	HWND hCombo, hWndHeight, hwndPP, hFrame, g_hFrame1, zoomfram, graphframe, pztGraphframe,maya;

	static HWND CreateButton(const wchar_t* text, int x, int y, int buttonWidth, int buttonHeight, HWND parent, int id, DWORD style = BS_PUSHBUTTON);
	static HWND CreateStaticText(const wchar_t* text, int x, int y, int width, int height, HWND parent, int id, DWORD style = WS_BORDER);
	static HWND MyUI::CreateSlider(int x, int y, int width, int height, HWND parent, int id, DWORD style);
	static HWND MyUI::InputSaveButton(const wchar_t* inputText, int x, int y, int w, int h, const wchar_t* buttonText, HWND parent, int inputId, int buttonId, DWORD inputStyle= WS_BORDER, DWORD buttonStyle= BS_PUSHBUTTON);
	HWND mainUi(HWND parent);
	HWND GetCamHandle() const;
	HWND GetZoomCamHandle() const;
	HWND GetBDgraphHandle() const;
	HWND GetPZTgraphHandle() const;
	HWND GetHeightTextHandle() const;
	HWND GetPZTTextHandle() const;
	HWND GetInputUthTextHandle() const;
	static void MyUI::mess(const wchar_t* text = L"Message");
	static INT_PTR CALLBACK CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK CustomDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SliderWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	std::string MyUI::GetInputText(HWND hInput);

};


#endif // !MY_UI_H
