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
#include "ExportCsv.h"


#define MAX_LOADSTRING 100

bool stopCamera = true;
HWND  hFrame;
std::deque<double> brightData;
Camera cam;
MyDaq daq;
ExportCSV csv;
ScreenRecord rec;
MyUI myUIInstance;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING], szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
std::atomic<bool> stopGraphUpdate(true);
std::atomic<double> pztVolt(0.0);

void mess(const wchar_t* text = L"Message") {
	MessageBoxW(NULL, text, L"Message", MB_OK | MB_ICONINFORMATION);
}

void GetFormattedDateTime(char* formattedDateTime, size_t bufferSize) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	_snprintf_s(formattedDateTime, bufferSize, _TRUNCATE, "%02d%02d%02d_%02d%02d%02d",
		st.wYear % 100, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void UpdateGraph() {
	const size_t bufferSize = 14;
	char formattedDateTime[bufferSize];
	GetFormattedDateTime(formattedDateTime, bufferSize);
	const char* fileName = formattedDateTime;

	while (!stopGraphUpdate) {
		std::deque<double> brightData = cam.GetBrightData();
		std::deque<double> pzt = cam.GetPZTvolt();
		PlotGraph pt;
		pt.completeOfGraph(myUIInstance.GetBDgraphHandle(), brightData,999);
		pt.completeOfGraph(myUIInstance.GetPZTgraphHandle(), pzt,10);
		if (cam.getDepositionBool()) {
			csv.saveCSV(brightData, pzt, fileName);
		}
		if (cam.getCaptureScreenBool()) {
			rec.CaptureAndSaveScreenshot(fileName);
			cam.setCaptureScreenBool(FALSE);
		}
		double bright = cam.getBrightness();
		if (myUIInstance.GetHeightTextHandle() != NULL) {
			std::wstring newText = L"Brightness: " + std::to_wstring(bright);
			SetWindowTextW(myUIInstance.GetHeightTextHandle(), newText.c_str());
		}
		double gg = cam.getUpdateofPzt();
		if (myUIInstance.GetPZTTextHandle() != NULL) {
			std::wstring newText = L"PZTvolt: " + std::to_wstring(gg);
			SetWindowTextW(myUIInstance.GetPZTTextHandle(), newText.c_str());
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
				//sr.CaptureAndSaveScreenshot(L"C:\\Users\\nares\\Desktop\\allout\\preference.bmp");
				//deep.setIsdeposition(false);
			}
			break;

			case ID_CAMERA_OPTION:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CAMERA_OPTIONS), hWnd, myUIInstance.CameraOptions);
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, myUIInstance.About);
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

