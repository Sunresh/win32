#include "framework.h"
#include <atomic>
#include <algorithm>
#include "Camera.h"
#include "PlotGraph.h"
#include "ScreenRecord.h"
#include "resource.h"
#include "framework.h"
#include <atomic>
#include "exportCSV.h"
#include <string>
#include <nidaqmx.h>
#include <iostream>
#include <string>
#include <vector>
#include "daq.h"
#include "brightnessCalculation.h"
#include <Windows.h>
#include "preferencemanager.h"
#include "MyUi.h"

#define MAX_LOADSTRING 100

bool stopCamera = true;
double meancv;
HWND hCombo, hWndHeight, hwndPP, hFrame;
std::deque<double> brightData;
Camera cam;
MyDaq daq;
MyUI myUIInstance;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING], szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void UpdateHeightText(HWND hWndStatic, double heightValue);
std::atomic<bool> stopGraphUpdate(true);
std::atomic<double> pztVolt(0.0);

long dep = 0.0001;
wchar_t buttonText[256];



void mess(const wchar_t* text = L"Message") {
	MessageBoxW(NULL, text, L"Message", MB_OK | MB_ICONINFORMATION);
}


void UpdateGraph() {
	while (!stopGraphUpdate) {
		std::deque<double> brightData = cam.GetBrightData();
		std::deque<double> pzt = cam.GetPZTvolt();
		PlotGraph pt;
		pt.completeOfGraph(myUIInstance.GetBDgraphHandle(), brightData,999);
		pt.completeOfGraph(myUIInstance.GetPZTgraphHandle(), pzt,10);
		double bright = cam.getBrightness();
		HWND hWndHeight = GetDlgItem(hFrame, IDC_STATIC_HEIGHT);
		if (hWndHeight != NULL) {
			std::wstring newText = L"Brightness: " + std::to_wstring(bright);
			SetWindowTextW(hWndHeight, newText.c_str());
		}
		double gg = cam.getUpdateofPzt();
		HWND hwndPP = GetDlgItem(hFrame, IDC_PPZZ);
		if (hwndPP != NULL) {
			std::wstring newText = L"PZTvolt: " + std::to_wstring(gg);
			SetWindowTextW(hwndPP, newText.c_str());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
		hFrame = myUIInstance.mainUi(hWnd);
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
				std::thread graphThread(UpdateGraph);
				graphThread.detach();

				std::thread displayThread([&]() {
					cam.DisplayCameraFrame(myUIInstance.GetCamHandle(), myUIInstance.GetZoomCamHandle());
					InvalidateRect(hWnd, NULL, TRUE);
					});
				displayThread.detach();

			}
			break;
			case ID_BTN_CAMERA_OFF:
			{
				cam.setStopCamera(true);
				stopCamera = true;
				stopGraphUpdate = true;
				
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
				ScreenRecord sr;
				sr.CaptureAndSaveScreenshot(L"C:\\Users\\nares\\Desktop\\allout\\preference.bmp");
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
		PreferenceManager pr;
		std::string loadedValue = pr.GetPreference("frame");
		HWND hwndPP = GetDlgItem(hWnd, ID_CAMERA_OPTION);
		if (hwndPP != NULL) {
			std::wstring wideLoadedValue(loadedValue.begin(), loadedValue.end());
			std::wstring newText = L"PZTvolt: " + wideLoadedValue;
			SetWindowTextW(hwndPP, newText.c_str());
		}

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

INT_PTR CALLBACK CustomDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
INT_PTR CALLBACK CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	PreferenceManager preferenceManager;

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
			if (IsDlgButtonChecked(hDlg, IDC_FRAME_RATE_30) == BST_CHECKED) {
				preferenceManager.SetPreference("frame", "3000");

			}
			else if (IsDlgButtonChecked(hDlg, IDC_FRAME_RATE_60) == BST_CHECKED) {
				preferenceManager.SetPreference("frame", "60");
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