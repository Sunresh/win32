#include "MyUi.h"

HWND MyUI::CreateButton(const wchar_t* text, int x, int y, int buttonWidth, int buttonHeight, HWND parent, int id, DWORD style)
{
	return CreateWindowW(L"BUTTON", text, WS_VISIBLE | WS_CHILD | style, x, y, buttonWidth, buttonHeight, parent, (HMENU)id, NULL, NULL);
}

HWND MyUI::CreateStaticText(const wchar_t* text, int x, int y, int width, int height, HWND parent, int id, DWORD style)
{
	return CreateWindowW(L"STATIC", text, WS_VISIBLE | WS_CHILD | style, x, y, width, height, parent, (HMENU)id, NULL, NULL);
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
	hWndHeight = CreateStaticText(L"Brightness: ", 2, 10 * buttonSpacing, buttonWidth, buttonHeight, hFrame, IDC_STATIC_HEIGHT);
	CreateStaticText(L"E.Volt: 5V", 2, 11 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	CreateStaticText(L"PZT volt: 10V", 2, 12 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	CreateStaticText(L"Dep. Time: 60s", 2, 13 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	CreateStaticText(L"Upper Th.: 75", 2, 14 * buttonSpacing, buttonWidth, buttonHeight, hFrame, NULL);
	CreateStaticText(L"Lower Th.: 25", 2, 15 * buttonSpacing, buttonWidth, buttonHeight, hFrame, IDC_YOUR_LOWER_TH_STATIC_ID);
	hwndPP = CreateStaticText(L"Lower Th.: 25", 2, 16 * buttonSpacing, buttonWidth, buttonHeight, hFrame, IDC_PPZZ);

	g_hFrame1 = CreateButton(L"Camera", buttonWidth, 2, camwidth, rowheight, hWnd, NULL, BS_GROUPBOX);
	zoomfram = CreateButton(L"ZOOM", buttonWidth + camwidth, 2, camwidth, rowheight, hWnd, NULL, BS_GROUPBOX);

	graphframe = CreateButton(L"Graph", buttonWidth, rowheight, 2 * camwidth, 0.25 * GetSystemMetrics(SM_CYSCREEN), hWnd, NULL, BS_GROUPBOX);
	pztGraphframe = CreateButton(L"Graph", buttonWidth, 0.25 * GetSystemMetrics(SM_CYSCREEN) + rowheight, 2 * camwidth, 0.15 * GetSystemMetrics(SM_CYSCREEN), hWnd, NULL, BS_GROUPBOX);
	return hFrame;
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

