#include "framework.h"
#include "WindowsProject1.h"
#include "opencv2/opencv.hpp"
#include <atomic>
#include <algorithm>
#define MAX_LOADSTRING 100

bool stopCamera = true;
double meancv;
HWND hCombo;
cv::VideoCapture cap;
std::deque<double> brightData;
Camera cam;
Deposition deep;
MyDaq daq;
HWND g_hFrame1;
HWND zoomfram,calFrame, info;
HWND graphframe, pztGraphframe;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void UpdateHeightText(HWND hWndStatic, double heightValue);
INT_PTR CALLBACK CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
std::atomic<bool> stopGraphUpdate(true);
std::atomic<double> pztVolt(0.0);

long dep = 0.0001;
wchar_t buttonText[256];


void UpdateGraph(HWND graphframe) {
	while (!stopGraphUpdate) {
		std::deque<double> brightData = cam.GetBrightData();
		std::deque<double> pzt = cam.GetPZTvolt();
		completeOfGraph(graphframe, brightData);
		completeOfGraph(pztGraphframe, pzt,10);
		double bright = cam.getBrightness();
		HWND hWndHeight = GetDlgItem(info, IDC_STATIC_HEIGHT);
		if (hWndHeight != NULL) {
			std::wstring newText = L"Brightness: " + std::to_wstring(bright);
			SetWindowTextW(hWndHeight, newText.c_str());
		}
		double gg = cam.getUpdateofPzt();
		HWND hwndPP = GetDlgItem(info, IDC_PPZZ);
		if (hwndPP != NULL) {
			std::wstring newText = L"PZTvolt: " + std::to_wstring(gg);
			SetWindowTextW(hwndPP, newText.c_str());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		const int buttonWidth = 126;
		const int buttonHeight = 30;
		const int buttonSpacing = 32;

		HWND hFrame = CreateWindowW(L"BUTTON", L"Menu", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 2, 2, 260, 215, hWnd, NULL, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Camera ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15, buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_CAMERA_ON, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Camera OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonWidth + 6, 15, buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_CAMERA_OFF, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Laser ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15 + (1 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_LASER_ON, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Laser OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonWidth+6, 15 + (1 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_LASER_OFF, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Deposition ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15 + (2 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_DEPOSITION_ON, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Dep Pause", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonWidth+6, 15 + (2 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_DEPOSITION_OFF, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Deposition OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15 + (3 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_DEPOSITION_OFF, NULL, NULL);
		hCombo = CreateWindowW(L"BUTTON", L"30", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15 + (4 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_CAMERA_OPTION, NULL, NULL);
		CreateWindowW(L"BUTTON", L"EPV0", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15 + (5 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_EPDV0, NULL, NULL);
		CreateWindowW(L"BUTTON", L"PZTV0", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonWidth + 6, 15 + (5 * buttonSpacing), buttonWidth, buttonHeight, hFrame, (HMENU)ID_BTN_PZTV0, NULL, NULL);

		g_hFrame1 = CreateWindowW(L"BUTTON", L"Camera", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 262, 2, 260, 215, hWnd, NULL, NULL, NULL);
		zoomfram = CreateWindowW(L"BUTTON", L"ZOOM", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 522, 2, 260, 215, hWnd, NULL, NULL, NULL);
		calFrame = CreateWindowW(L"BUTTON", L"Calculation Area", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 782, 2, 260, 215, hWnd, NULL, NULL, NULL);
		info = CreateWindowW(L"BUTTON", L"Information", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 2, 215, 260, 415, hWnd, NULL, NULL, NULL);
		
		HWND hWndHeight = CreateWindowW(L"STATIC", L"Brightness: ", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 20, 240, 20, info, (HMENU)IDC_STATIC_HEIGHT, NULL, NULL);
		
		CreateWindowW(L"STATIC", L"E.Volt: 5V", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 50, 240, 20, info, NULL, NULL, NULL);
		CreateWindowW(L"STATIC", L"PZT volt: 10V", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 80, 240, 20, info, NULL, NULL, NULL);
		CreateWindowW(L"STATIC", L"Dep. Time: 60s", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 110, 240, 20, info, NULL, NULL, NULL);
		CreateWindowW(L"STATIC", L"Upper Th.: 75", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 140, 240, 20, info, NULL, NULL, NULL);
		CreateWindowW(L"STATIC", L"Lower Th.: 25", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 170, 240, 20, info, (HMENU)IDC_YOUR_LOWER_TH_STATIC_ID, NULL, NULL);
		HWND hwndPP =CreateWindowW(L"STATIC", L"Lower Th.: 25", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 200, 240, 20, info, (HMENU)IDC_PPZZ, NULL, NULL);

		graphframe = CreateWindowW(L"BUTTON", L"Graph", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 262, 215, 620, 215, hWnd, NULL, NULL, NULL);
		pztGraphframe = CreateWindowW(L"BUTTON", L"Graph", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 262, 430, 620, 215, hWnd, NULL, NULL, NULL);

		SetWindowLongPtr(hFrame, GWLP_WNDPROC, (LONG_PTR)WndProc);
	}
	break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case ID_BTN_CAMERA_ON:
			{
				cam.setStopCamera(false);
				stopGraphUpdate = false;
				std::thread graphThread(UpdateGraph, graphframe);
				graphThread.detach();

				std::thread displayThread([&]() {
					cam.DisplayCameraFrame(g_hFrame1, zoomfram, calFrame);
					InvalidateRect(hWnd, NULL, TRUE);
					});
				displayThread.detach();

			}
			break;
			case ID_BTN_CAMERA_OFF:
			{
				cam.setStopCamera(true);
				stopCamera = true;
				stopGraphUpdate = true; // Stop updating the graph
				
			}
				break;
			case ID_BTN_LASER_ON:
			{
				daq.addDigitalChannel("Dev2/port0/line0");
				daq.digitalOut("Dev2/port0/line0", true);
				daq.startTasks();
			}
			break;
			case ID_BTN_LASER_OFF:
			{
				daq.addDigitalChannel("Dev2/port0/line0");
				daq.digitalOut("Dev2/port0/line0", false);
				daq.startTasks();
			}
			break;
			case ID_BTN_EPDV0:
			{
				daq.addAnalogChannel("Dev2/ao0");
				daq.analogOut("Dev2/ao0", 0.0);
				daq.startTasks();
			}
				break;
			case ID_BTN_PZTV0:
			{
				daq.addAnalogChannel("Dev2/ao1");
				daq.analogOut("Dev2/ao1", 0.0);
				daq.startTasks();
			}
			break;
			case ID_BTN_DEPOSITION_ON:
			{
				cam.setDepositionBool(true);
				//std::thread deposiiiii([&]() {
				//	deep.setIsdeposition(true);
				//	deep.depositionFunction(pztGraphframe, info);
				//	InvalidateRect(hWnd, NULL, TRUE);
				//	});
				//deposiiiii.detach();
			}
			break;
			case ID_BTN_DEPOSITION_OFF:
			{
				cam.setDepositionBool(false);
				//deep.setIsdeposition(false);
			}
			break;

			case ID_CAMERA_OPTION:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CAMERA_OPTIONS), hWnd, CameraOptions);
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

void UpdateHeightText(HWND hWndStatic, double heightValue) {
	wchar_t buffer[50];
	swprintf_s(buffer, sizeof(buffer) / sizeof(buffer[0]), L"Height: %.2f", heightValue);
	SetWindowTextW(hWndStatic, buffer);
	RedrawWindow(hWndStatic, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}

INT_PTR CALLBACK CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
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
			// Determine which frame rate option is selected
			if (IsDlgButtonChecked(hDlg, IDC_FRAME_RATE_30) == BST_CHECKED) {
				const std::wstring br = L"30"; // Replace this with the appropriate value
				if (hCombo != NULL) {
					SetWindowTextW(hCombo, br.c_str());
				}
			}
			else if (IsDlgButtonChecked(hDlg, IDC_FRAME_RATE_60) == BST_CHECKED) {
				const std::wstring br = L"60"; // Replace this with the appropriate value
				if (hCombo != NULL) {
					SetWindowTextW(hCombo, br.c_str());
				}
			}
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}
