#include "framework.h"
#include <atomic>
#include <algorithm>
#include "Camera.h"
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
#include <memory>


#define MAX_LOADSTRING 100

HWND  hFrame;
std::deque<double> brightData;
MyDaq daq;
TaskHandle lserOn = nullptr;
TaskHandle lserOff = nullptr;
ExportCSV csv;
ScreenRecord rec;
MyUI myUIInstance;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING], szWindowClass[MAX_LOADSTRING];

std::mutex mtx;

std::unique_ptr<Camera> camPtr;
std::unique_ptr<PreferenceManager> pref;
bool laserState = false;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
std::atomic<double> pztVolt(0.0);

void yymmdd_hhmmss(char* formattedDateTime, size_t bufferSize) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	_snprintf_s(formattedDateTime, bufferSize, _TRUNCATE, "%02d%02d%02d_%02d%02d%02d",
		st.wYear % 100, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
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
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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
   const int x = std::round(0.50 * GetSystemMetrics(SM_CXSCREEN));
   const int y = std::round(0.05 * GetSystemMetrics(SM_CYSCREEN));
   const int w = std::round(0.90 * GetSystemMetrics(SM_CXSCREEN));
   const int h = std::round(0.90 * GetSystemMetrics(SM_CYSCREEN));

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0,y, w, h, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
void pushh(int controlId,std::string key) {
	HWND hInput = GetDlgItem(hFrame, controlId);

	// Get the length of the text
	int textLength = GetWindowTextLengthW(hInput) + 1;

	// Allocate a buffer to hold the text
	wchar_t* buffer = new wchar_t[textLength];

	// Get the text from the input control
	GetWindowTextW(hInput, buffer, textLength);

	// Convert to std::string
	std::string result(buffer, buffer + textLength - 1); // Exclude null terminator from the length
	// Clean up
	delete[] buffer;

	PreferenceManager pref;
	pref.SetPreference(key, result);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PreferenceManager pref;
    switch (message)
    {
	case WM_FRAMERATE_UPDATED:
	{
		myUIInstance.messi("rrrr");
	}
		// Update frame rate
	break;

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
			case ID_BTN_CAMERA_ON: {
				PreferenceManager pref;
				if (pref.getprefString(CameraONoFF) == "OFF") {
					pref.SetPreference(CameraONoFF, "ON");
					if (pref.SetPreference(CURRENT_FILENAME_KEY, myUIInstance.yymmdd_hhmmss())) {
						camPtr = std::make_unique<Camera>();
						camPtr->DisplayCameraFrame(myUIInstance.getCamCam());
					}
				}
				
				break;
			}
			case ID_BTN_CAMERA_OFF: {
				if (pref.getprefString(CameraONoFF) == "ON") {
					pref.SetPreference(CameraONoFF, "OFF");
					camPtr->setStopCamera(FALSE);
				}
				break;
			}
			case ID_BTN_CAMERA_CAMAC: {
				 for (int i = 0; i < 30; ++i) {  // Checking up to 10 camera indexes
						cv::VideoCapture cap(i);
						if (cap.isOpened()) {
							std::string ff = std::to_string(i);
							myUIInstance.messi(ff);
							cap.release();  // Release the VideoCapture
						} else {
							std::cout << "Camera index " << i << " is not available." << std::endl;
						}
					}
				break;
			}
			case ID_BTN_CAMERA_RE: {
				PreferenceManager pref;
				if (pref.getprefString(CameraONoFF) == "ON") {
					camPtr->setStopCamera(FALSE);
					if (pref.SetPreference(CURRENT_FILENAME_KEY, myUIInstance.yymmdd_hhmmss())) {
						camPtr = std::make_unique<Camera>();
						camPtr->DisplayCameraFrame(myUIInstance.getCamCam());
					}
				}
				else if (pref.getprefString(CameraONoFF) == "OFF") {
					pref.SetPreference(CameraONoFF, "ON");
					if (pref.SetPreference(CURRENT_FILENAME_KEY, myUIInstance.yymmdd_hhmmss())) {
						camPtr = std::make_unique<Camera>();
						camPtr->DisplayCameraFrame(myUIInstance.getCamCam());
					}
				}
				
				break;
			}

			case ID_BTN_LASER_ON:
			{
				TaskHandle taskHandle;
				DAQmxCreateTask("", &taskHandle);
				DAQmxCreateDOChan(taskHandle, "Dev2/port0/line0", "", DAQmx_Val_ChanForAllLines);
				uInt32 data = (laserState ? false : true);
				DAQmxWriteDigitalU32(taskHandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, &data, nullptr, nullptr);
				DAQmxStopTask(taskHandle);
				DAQmxClearTask(taskHandle);

				laserState = !laserState; // Toggle the laser state
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
				if (pref.SetPreference(CURRENT_FILENAME_KEY, myUIInstance.yymmdd_hhmmss())) {
					camPtr->setDepositionBool(TRUE);
				}				
			}
			break;
			case ID_BTN_DEPOSITION_OFF:
			{
				camPtr->setDepositionBool(FALSE);
				ScreenRecord sr;
				sr.CaptureAndSaveScreenshot("C:\\Users\\nares\\Desktop\\allout\\preference.bmp");
			}
			break;
			case BTN_UTH:
			{
				pushh(INPUT_UTH, UTH_KEY);
			}
			break;
			case BTN_LTH:
			{
				pushh(INPUT_LTH, LTH_KEY);
			}
			break;
			case BTN_PZT:
			{
				pushh(INPUT_PZT, PZT_KEY);
			}
			break;
			case BTN_SQH:
			{
				pushh(INPUT_SQH, SQH_KEY);
			}
			break;
			case BTN_SQW:
			{
				pushh(INPUT_SQW, SQW_KEY);
			}
			break;
			case BTN_SQX:
			{
				pushh(INPUT_SQX, SQX1_KEY);
			}
			break;
			case BTN_SQY:
			{
				pushh(INPUT_SQY, SQY1_KEY);
			}
			break;
			case BTN_MSQX1:
			{
				pushh(INPUT_MSQX1, MSQX1_KEY);
			}
			break;
			case BTN_MSQY1:
			{
				pushh(INPUT_MSQY1, MSQY1_KEY);
			}
			break;
			case BTN_MSQX2:
			{
				pushh(INPUT_MSQX2, MSQX2_KEY);
			}
			break;
			case BTN_MSQY2:
			{
				pushh(INPUT_MSQY2, MSQY2_KEY);
			}
			break;
			case BTN_TIME:
			{
				pushh(INPUT_TIME, TIME_KEY);
			}
			break;
			
			case CAM_INDEX_BTN:
			{
				pushh(CAM_INDEX_INPUT, CameraIndex);
			}
			break;
			case TGL_BTN_GRAPH:
			{
				(pref.getprefString(AUTOGRAPH_KEY) == "on") ? pref.SetPreference(AUTOGRAPH_KEY, "off") : pref.SetPreference(AUTOGRAPH_KEY, "on");
			}
			break;
			case TGL_BTN_RECORD:
			{
				(pref.getprefString(AUTORECORD_KEY) == "on") ? pref.SetPreference(AUTORECORD_KEY, "off") : pref.SetPreference(AUTORECORD_KEY, "on");
			}
			break;
			case TGL_BTN_SDORDIFF:
			{
				(pref.getprefString(ADORDIFF_KEY) == "on") ? pref.SetPreference(ADORDIFF_KEY, "off") : pref.SetPreference(ADORDIFF_KEY, "on");
			}
			break;
			case TGL_BTN_RBOX:
			{
				(pref.getprefString(EBOXONOFF_KEY) == "on") ? pref.SetPreference(EBOXONOFF_KEY, "off") : pref.SetPreference(EBOXONOFF_KEY, "on");
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
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY: {
		pref.SetPreference(CameraONoFF, "OFF");
		if (camPtr) {
			camPtr->setStopCamera(FALSE); // Stop the camera
		}
		PostQuitMessage(0);
	}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

