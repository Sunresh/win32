#ifndef MY_UI_H
#define MY_UI_H
#include <Windows.h>
#include "Resource.h"
#include <iostream>
#include <CommCtrl.h>
#include "preferencemanager.h"

class MyUI {
public:
	HWND hCombo, g_hFrame1, zoomfram, graphframe, pztGraphframe;
	HWND btnUth,btnLth,btnPZT,btnSQH,btnSQW,btnSQX,btnSQY,btnTIME;
	HWND txtBD, txtEVOLT,txtPZT,txtTIME,txtUTH,txtLTH;
	HWND hFrame;
	static HWND CreateButton(const wchar_t* text, int x, int y, int buttonWidth, int buttonHeight, HWND parent, int id, DWORD style = BS_PUSHBUTTON);
	static HWND CreateStaticText(const wchar_t* text, int x, int y, int width, int height, HWND parent, int id, DWORD style = WS_BORDER);
	static HWND MyUI::CreateSlider(int x, int y, int width, int height, HWND parent, int id, DWORD style);
	static HWND MyUI::InputSaveButton(const wchar_t* inputText, int x, int y, int w, int h, const wchar_t* buttonText, HWND parent, int inputId, int buttonId, DWORD inputStyle= WS_BORDER, DWORD buttonStyle= BS_PUSHBUTTON);
	HWND mainUi(HWND parent);
	void MyUI::loadPref(HWND hwnd, std::string key);
	void MyUI::loadPrefv(HWND hwnd, std::string key);

	HWND GetCamHandle() const;
	HWND GetZoomCamHandle() const;
	HWND GetBDgraphHandle() const;
	HWND GetPZTgraphHandle() const;

	HWND getTxtBD() const;
	HWND getTxtUth() const;
	HWND getTxtLth() const;
	HWND getTxtEVOLT() const;
	HWND getTxtPZT() const;
	HWND getTxtTIME() const;

	HWND getInputUth() const;
	HWND getInputlth() const;
	HWND getInputPZT() const;
	HWND getInputSQH() const;
	HWND getInputSQW() const;
	HWND getInputSQX() const;
	HWND getInputSQY() const;
	HWND getInputTIME() const;

	static void MyUI::mess(const wchar_t* text = L"Message");
	static void MyUI::messi(std::string text = "Message");

	static INT_PTR CALLBACK CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK CustomDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SliderWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	std::string MyUI::GetInputText(HWND hInput);
	void MyUI::btnh(HWND input, HWND output, std::string key);

};


#endif // !MY_UI_H
