#include "MyUi.h"


HWND MyUI::CreateButton(const wchar_t* text, int x, int y, int buttonWidth, int buttonHeight, HWND parent, int id, DWORD style)
{
	return CreateWindowW(L"BUTTON", text, WS_VISIBLE | WS_CHILD | style, x, y, buttonWidth, buttonHeight, parent, (HMENU)id, NULL, NULL);
}

HWND MyUI::CreateStaticText(const wchar_t* text, int x, int y, int width, int height, HWND parent, int id, DWORD style)
{
	return CreateWindowW(L"STATIC", text, WS_VISIBLE | WS_CHILD | style, x, y, width, height, parent, (HMENU)id, NULL, NULL);
}

HWND MyUI::InputSaveButton(const wchar_t* inputText, int x, int y, int w, int h, const wchar_t* buttonText, HWND parent, int inputId, int buttonId, DWORD inputStyle, DWORD buttonStyle)
{
	HWND hInput = CreateWindowW(L"EDIT", inputText, WS_VISIBLE | WS_CHILD | inputStyle,x, y, w/2, h, parent, (HMENU)inputId, NULL, NULL);
	HWND hButton = CreateWindowW(L"BUTTON", buttonText, WS_VISIBLE | WS_CHILD | buttonStyle,x+w/2, y, w/2, h, parent, (HMENU)buttonId, NULL, NULL);
	return hInput;
}

std::string MyUI::GetInputText(HWND hInput)
{
	// Get the length of the text
	int textLength = GetWindowTextLengthW(hInput) + 1;

	// Allocate a buffer to hold the text
	wchar_t* buffer = new wchar_t[textLength];

	// Get the text from the input control
	GetWindowTextW(hInput, buffer, textLength);

	// Convert to std::wstring
	std::string result(buffer, buffer + textLength);
	// Clean up
	delete[] buffer;

	return result;
}

std::string MyUI::getTextInput(HWND hInput)
{
	// Get the length of the text
	int textLength = GetWindowTextLengthW(hInput) + 1;

	// Allocate a buffer to hold the text
	wchar_t* buffer = new wchar_t[textLength];

	// Get the text from the input control
	GetWindowTextW(hInput, buffer, textLength);

	// Convert to std::wstring
	std::string result(buffer, buffer + textLength);
	// Clean up
	delete[] buffer;

	return result;
}

HWND MyUI::mainUi(HWND hWnd) {
	const int buttonWidth = std::round(0.1 * GetSystemMetrics(SM_CXSCREEN));
	const int camwidth = std::round(0.4 * GetSystemMetrics(SM_CXSCREEN));
	const int buttonHeight = std::round(0.05 * GetSystemMetrics(SM_CYSCREEN));
	const int buttonSpacing = buttonHeight + 2;
	const int rowheight = std::round(0.45 * GetSystemMetrics(SM_CYSCREEN));

	hFrame = CreateButton(L"Menu", 1, 1, 260, GetSystemMetrics(SM_CYSCREEN), hWnd, NULL, BS_GROUPBOX);
	CreateButton(L"Camera ON", 2, 1, buttonWidth, buttonHeight, hFrame, ID_BTN_CAMERA_ON);
	CreateButton(L"Camera OFF", 2, buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_CAMERA_OFF);
	CreateButton(L"Laser ON", 2, 2 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_LASER_ON);
	CreateButton(L"Laser OFF", 2, 3 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_LASER_OFF);
	CreateButton(L"Deposition ON", 2, 4 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_DEPOSITION_ON);
	CreateButton(L"Dep Pause", 2, 5 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_DEPOSITION_OFF);
	CreateButton(L"Deposition OFF", 2, 6 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_DEPOSITION_OFF);
	hCombo = CreateButton(L"30", 2, 7 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_CAMERA_OPTION);
	CreateButton(L"EPV0", 2, 8 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_EPDV0);
	CreateButton(L"PZTV0", 2, 9 * buttonSpacing, buttonWidth, buttonHeight, hFrame, ID_BTN_PZTV0);

	txtBD = CreateStaticText(L"Brightness: ", 2, 10 * buttonSpacing, buttonWidth, buttonHeight, hFrame, IDC_STATIC_HEIGHT);
	loadPref(txtBD,"txtBD");
	txtEVOLT = CreateStaticText(L"E.Volt:", 2, 11 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	loadPref(txtEVOLT, "txtBD");
	txtPZT = CreateStaticText(L"PZT volt:", 2, 12 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	loadPref(txtPZT, "PZT");
	txtTIME = CreateStaticText(L"Dep. Time(s)", 2, 13 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	loadPref(txtTIME, "TIME");
	txtUTH = CreateStaticText(L"Upper Th.:", 2, 14 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	loadPref(txtUTH, "LowerLH");
	txtLTH = CreateStaticText(L"Lower Th.:", 2, 15 * buttonSpacing, buttonWidth, buttonHeight, hFrame, IDC_YOUR_LOWER_TH_STATIC_ID);
	loadPref(txtLTH, "LowerLH");
	//hwndPP = CreateStaticText(L"Lower Th.:", 2, 16 * buttonSpacing, buttonWidth, buttonHeight, hFrame, IDC_PPZZ);

	g_hFrame1 = CreateButton(L"Camera", buttonWidth, 2, camwidth, rowheight, hWnd, NULL, BS_GROUPBOX);
	zoomfram = CreateButton(L"ZOOM", buttonWidth + camwidth, 2, camwidth, rowheight, hWnd, NULL, BS_GROUPBOX);

	graphframe = CreateButton(L"Graph", buttonWidth, rowheight, 2 * camwidth, 0.25 * GetSystemMetrics(SM_CYSCREEN), hWnd, NULL, BS_GROUPBOX);
	pztGraphframe = CreateButton(L"Graph", buttonWidth, 0.25 * GetSystemMetrics(SM_CYSCREEN) + rowheight, 2 * camwidth, 0.15 * GetSystemMetrics(SM_CYSCREEN), hWnd, NULL, BS_GROUPBOX);

	// Assume MyUI is an instance of your UI class
	CreateSlider(buttonWidth+2* camwidth, 1, buttonWidth, 3*buttonHeight, hWnd, IDC_SLIDER, TBS_AUTOTICKS | TBS_ENABLESELRANGE);
	btnUth = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 3 * buttonHeight, buttonWidth, buttonHeight, L"Save Uth", hWnd, INPUT_UTH, BTN_UTH, NULL);
	loadPrefv(btnUth, "UpperTH");
	btnLth = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 4 * buttonHeight, buttonWidth, buttonHeight, L"Save Lth", hWnd, INPUT_LTH, BTN_LTH, NULL);
	loadPrefv(btnLth, "LowerTH");
	btnPZT = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 5 * buttonHeight, buttonWidth, buttonHeight, L"Save PZT", hWnd, INPUT_PZT, BTN_PZT, NULL);
	loadPrefv(btnPZT, "PZT");
	btnSQH = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 6 * buttonHeight, buttonWidth, buttonHeight, L"Save SH", hWnd, INPUT_SQH, BTN_SQH, NULL);
	loadPrefv(btnSQH, "SQH");
	btnSQW = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 7 * buttonHeight, buttonWidth, buttonHeight, L"Save SW", hWnd, INPUT_SQW, BTN_SQW, NULL);
	loadPrefv(btnSQW, "SQW");
	btnSQX = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 8 * buttonHeight, buttonWidth, buttonHeight, L"Save SX", hWnd, INPUT_SQX, BTN_SQX, NULL);
	loadPrefv(btnSQX, "SQX");
	btnSQY = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 9 * buttonHeight, buttonWidth, buttonHeight, L"Save SY", hWnd, INPUT_SQY, BTN_SQY, NULL);
	loadPrefv(btnSQY, "SQY");
	btnTIME = InputSaveButton(L"0", buttonWidth + 2 * camwidth, 10 * buttonHeight, buttonWidth, buttonHeight, L"Save Time", hWnd, INPUT_TIME, BTN_TIME, NULL);
	loadPrefv(btnTIME, "TIME");

	return hFrame;
}

void MyUI::loadPref(HWND hwnd, std::string key) {
	PreferenceManager pref;
	std::string value = pref.getprefString(key);
	std::stringstream newTextStream;
	newTextStream << key << ":" << value;
	std::string text = newTextStream.str();
	std::wstring newText(text.begin(), text.end());
	SetWindowTextW(hwnd, newText.c_str());
}
void MyUI::loadPrefv(HWND hwnd, std::string key) {
	PreferenceManager pref;
	std::stringstream newTextStream;
	newTextStream << pref.getprefString(key);
	std::string text = newTextStream.str();
	std::wstring newText(text.begin(), text.end());
	SetWindowTextW(hwnd, newText.c_str());
}


HWND MyUI::GetCamHandle() const {
	return g_hFrame1;
}
HWND MyUI::GetZoomCamHandle() const {
	return zoomfram;
}
HWND MyUI::GetBDgraphHandle() const {
	return graphframe;
}
HWND MyUI::GetPZTgraphHandle() const {
	return pztGraphframe;
}

HWND MyUI::getTxtBD() const {
	return txtBD;
}
HWND MyUI::getTxtUth() const {
	return txtUTH;
}
HWND MyUI::getTxtLth() const {
	return txtLTH;
}
HWND MyUI::getTxtEVOLT() const {
	return txtEVOLT;
}
HWND MyUI::getTxtPZT() const {
	return txtPZT;
}
HWND MyUI::getTxtTIME() const {
	return txtTIME;
}


HWND MyUI::getInputUth() const {
	return btnUth;
}
HWND MyUI::getInputlth() const {
	return btnLth;
}
HWND MyUI::getInputPZT() const {
	return btnPZT;
}
HWND MyUI::getInputSQH() const {
	return btnSQH;
}
HWND MyUI::getInputSQW() const {
	return btnSQW;
}
HWND MyUI::getInputSQX() const {
	return btnSQX;
}
HWND MyUI::getInputSQY() const {
	return btnSQY;
}
HWND MyUI::getInputTIME() const {
	return btnTIME;
}

void MyUI::pushh(HWND hdmi,int input, std::string key) {
	PreferenceManager pref;
	wchar_t buffer[256];
	UINT textLength = GetDlgItemTextW(hdmi, input, buffer, sizeof(buffer) / sizeof(buffer[0]));
	if (textLength > 0) {
		wchar_t* text = buffer;
		pref.SetPreferenceW(key, text);
	}
	else {}
}

INT_PTR CALLBACK MyUI::CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	PreferenceManager pref;
		HWND hubtn = GetDlgItem(hDlg, ST_INP_UTH);
		HWND hlbtn = GetDlgItem(hDlg, ST_INP_LTH);
		HWND hpbtn = GetDlgItem(hDlg, ST_INP_PZT);
		HWND hebtn = GetDlgItem(hDlg, ST_INP_EPV);
		HWND htbtn = GetDlgItem(hDlg, ST_INP_TIME);
		HWND sqwval = GetDlgItem(hDlg, ST_INP_SQW);
		HWND sqhval = GetDlgItem(hDlg, ST_INP_SQH);
		HWND sqx1p = GetDlgItem(hDlg, ST_INP_SQX1);
		HWND sqy1p = GetDlgItem(hDlg, ST_INP_SQY1);

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case (IDC_CANCEL):
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDC_APPLY:
			loadPrefv(hubtn, UTH_KEY);
			loadPrefv(hlbtn, LTH_KEY);
			loadPrefv(hpbtn, PZT_KEY);
			loadPrefv(hebtn, EPV_KEY);
			loadPrefv(htbtn, TIME_KEY);
			loadPrefv(sqwval, SQW_KEY);
			loadPrefv(sqhval, SQH_KEY);
			loadPrefv(sqx1p, SQX1_KEY);
			loadPrefv(sqy1p, SQY1_KEY);
			break;

		case IDC_SAVEALL:
			if (MessageBoxW(NULL, L"Are you sure?", L"Confirm", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK) {
				pushh(hDlg, ST_INP_UTH, UTH_KEY);
				pushh(hDlg, ST_INP_LTH, LTH_KEY);
				pushh(hDlg, ST_INP_PZT, PZT_KEY);
				pushh(hDlg, ST_INP_EPV, EPV_KEY);
				pushh(hDlg, ST_INP_TIME, TIME_KEY);
				pushh(hDlg, ST_INP_SQW, SQW_KEY);
				pushh(hDlg, ST_INP_SQH, SQH_KEY);
				pushh(hDlg, ST_INP_SQX1, SQX1_KEY);
				pushh(hDlg, ST_INP_SQY1, SQY1_KEY);
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			else {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			
			break;
		case ST_BTN_UTH:
			pushh(hDlg, ST_INP_UTH, UTH_KEY);
			break;

		case ST_BTN_LTH:
			pushh(hDlg, ST_INP_LTH, LTH_KEY);
			break;

		case ST_BTN_PZT:
			pushh(hDlg, ST_INP_PZT, PZT_KEY);
			break;

		case ST_BTN_EPV:
			pushh(hDlg, ST_INP_EPV, EPV_KEY);
			break;

		case ST_BTN_TIME:
			pushh(hDlg, ST_INP_TIME, TIME_KEY);
			break;
		case ST_BTN_SQW:
			pushh(hDlg, ST_INP_SQW, SQW_KEY);
			break;

		case ST_BTN_SQH:
			pushh(hDlg, ST_INP_SQH, SQH_KEY);
			break;
		case ST_BTN_SQX1:
			pushh(hDlg, ST_INP_SQX1, SQX1_KEY);
			break;

		case ST_BTN_SQY1:
			pushh(hDlg, ST_INP_SQY1, SQY1_KEY);
			break;

		default:
			break;

		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MyUI::CustomDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE; // Focus is set to the edit control on dialog initialization

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			// OK button clicked, retrieve text from the edit control
			wchar_t buffer[256];
			GetDlgItemTextW(hwndDlg, IDC_EDIT_INPUT, buffer, sizeof(buffer) / sizeof(buffer[0]));

			// Process the input as needed
			MessageBoxW(NULL, buffer, L"User Input", MB_OK | MB_ICONINFORMATION);

			// Close the dialog
			EndDialog(hwndDlg, IDOK);
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			// Cancel button clicked
			EndDialog(hwndDlg, IDCANCEL);
		}
		return TRUE; // Message handled
	}
	return FALSE; // Message not handled
}

INT_PTR CALLBACK MyUI::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HWND MyUI::CreateSlider(int x, int y, int width, int height, HWND parent, int id, DWORD style)
{
	InitCommonControls();

	// Create the slider control
	HWND slider = CreateWindowW(L"msctls_trackbar32", L"", WS_VISIBLE | WS_CHILD | style, x, y, width, height, parent, (HMENU)id, NULL, NULL);

	// Create a static text control for the label
	CreateWindowW(L"STATIC", L"Slider Value:", WS_VISIBLE | WS_CHILD, x, y+ height, 100, 20, parent, NULL, NULL, NULL);

	// Create a static text control to display the current value
	HWND valueLabel = CreateWindowW(L"STATIC", L"0", WS_VISIBLE | WS_CHILD, x + width + 10, y, 50, 20, parent, NULL, NULL, NULL);

	// Set the value label as user data of the slider control
	SetWindowLongPtr(slider, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(valueLabel));

	// Set the initial position for the value label
	SetWindowTextW(valueLabel, L"0");

	// Set up a notification handler for the slider
	SendMessage(slider, TBM_SETBUDDY, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(valueLabel));
	SendMessage(slider, TBM_SETRANGE, static_cast<WPARAM>(TRUE), MAKELONG(0, 10));  // Set the range from 0 to 10
	SendMessage(slider, TBM_SETTICFREQ, 1, 0);
	// Add a message handler for WM_HSCROLL to update the value label
	SetWindowSubclass(slider, SliderWndProc, 0, 0);
	return slider;
}

LRESULT CALLBACK MyUI::SliderWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{

	if (uMsg == WM_HSCROLL)
	{
		HWND valueLabel = reinterpret_cast<HWND>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		// Get the current position of the slider
		int currentPosition = SendMessage(hwnd, TBM_GETPOS, 0, 0);

		// Convert the position to a string and update the value label
		wchar_t valueText[10];
		swprintf(valueText, L"%d", currentPosition);
		SetWindowTextW(valueLabel, valueText);
		OutputDebugStringW(L"\n\nSliderWndProc called\n\n");
	}

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void MyUI::mess(const wchar_t* text) {
	MessageBoxW(NULL, text, L"Message", MB_OK | MB_ICONINFORMATION);
}
void MyUI::messi(std::string text) {
	std::stringstream newTextStream;
	newTextStream << text;
	std::string texty = newTextStream.str();
	std::wstring newText(texty.begin(), texty.end());
	MessageBoxW(NULL, newText.c_str(), L"Message", MB_OK | MB_ICONINFORMATION);
}

void MyUI::btnh(HWND input, HWND output, std::string key) {
	PreferenceManager pref;
	std::string ma = GetInputText(input);
	pref.SetPreference(key, ma);
	if (HWND pztTextHandle = output)
	{
		std::stringstream newTextStream;
		newTextStream << key << ":" << ma;
		std::string text = newTextStream.str();
		std::wstring newText(text.begin(), text.end());
		SetWindowTextW(pztTextHandle, newText.c_str());
		//myUIInstance.mess(newText.c_str());
	}

}
