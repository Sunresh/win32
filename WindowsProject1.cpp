#include "framework.h"
#include "WindowsProject1.h"
#include "opencv2/opencv.hpp"
#include "camera.cpp"
#include "daqSignal.h"
#include "Deposition.h"
#include "MasterThread.h"

#define MAX_LOADSTRING 100
#define ID_BTN_CAMERA_ON 130
#define ID_BTN_CAMERA_OFF 131
#define ID_BTN_LASER_ON 132
#define ID_BTN_LASER_OFF 133
#define IDC_STATIC_HEIGHT 134
#define ID_BTN_DEPOSITION_ON 135
#define ID_BTN_DEPOSITION_OFF 136
#define IDC_YOUR_LOWER_TH_STATIC_ID 137
#define ID_CAMERA_OPTION 138

bool stopCamera = true;
double meancv;
cv::VideoCapture cap;
std::deque<double> brightData;
Camera cam;
MyDaq daq;
Deposition Dp;
MasterThread mThread;
HWND g_hFrame1;
HWND zoomfram,calFrame, info;
HWND graphframe;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void UpdateHeightText(HWND hWndStatic, double heightValue);

std::atomic<bool> stopGraphUpdate(true);
std::atomic<bool> isDeposition(true);
double pztVolt = 0.0;
double steps = 1000;
bool isIncrease = true;
long dep = 10;
void depositionFunction() {
	while (isDeposition) {
		if (pztVolt < 4 && isIncrease) {
			pztVolt += 1 / steps;
		}
		if (pztVolt >= 4.00) {
			pztVolt = 3.999999;
			isIncrease = false;
		}
		if (pztVolt < 0) {
			pztVolt = 0;
			return;
		}
		if (pztVolt > 0 && !isIncrease) {
			pztVolt -= 1 / steps;
		}
		HWND hWndLowerTh = GetDlgItem(info, IDC_YOUR_LOWER_TH_STATIC_ID);
		if (hWndLowerTh != NULL) {
			std::wstring newText = L"Lower Th.: " + std::to_wstring(pztVolt);
			SetWindowTextW(hWndLowerTh, newText.c_str());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(dep)); 
	}
}

void UpdateGraph(HWND graphframe) {
	const std::deque<double>& brightData = cam.GetBrightData();
	Dp.start();
	while (!stopGraphUpdate) {
		if (!brightData.empty()) {
			HDC graphf = GetDC(graphframe);
			RECT rect;
			GetClientRect(graphframe, &rect);
			FillRect(graphf, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

			// Drawing code for X-axis (horizontal line at the center)
			HPEN hXAxisPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200)); // Adjust color as needed
			SelectObject(graphf, hXAxisPen);
			MoveToEx(graphf, rect.left, rect.bottom / 2, NULL);
			LineTo(graphf, rect.right, rect.bottom / 2);

			// Drawing code for Y-axis (vertical line at the center)
			MoveToEx(graphf, rect.right / 2, rect.top, NULL);
			LineTo(graphf, rect.right / 2, rect.bottom);

			// Drawing code for the graph
			HPEN hGraphPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // Adjust graph color as needed
			SelectObject(graphf, hGraphPen);

			int dataCount = static_cast<int>(brightData.size());
			double scaleY = 0.8; // Adjust this scale factor as needed
			int startX = 0;

			for (int x = 0; x < rect.right; ++x) {
				int dataIndex = static_cast<int>((static_cast<double>(x) / rect.right) * dataCount);
				double y = brightData[dataIndex] * scaleY + 10; // Adjust additional offsets as needed

				int startY = rect.bottom - static_cast<int>(y);
				if (x == 0) {
					MoveToEx(graphf, startX, startY, NULL);
				}
				else {
					LineTo(graphf, x, startY);
				}
				startX = x;
			}

			DeleteObject(hXAxisPen);
			DeleteObject(hGraphPen);
			ReleaseDC(graphframe, graphf);
		}
		double bright = cam.getBrightness();
		HWND hWndHeight = GetDlgItem(info, IDC_STATIC_HEIGHT);
		if (hWndHeight != NULL) {
			std::wstring newText = L"Brightness: " + std::to_wstring(bright);
			SetWindowTextW(hWndHeight, newText.c_str());
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust the delay as needed
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
		const int buttonWidth = 255;
		const int buttonHeight = 30;
		const int buttonSpacing = 32;

		HWND hFrame = CreateWindowW(L"BUTTON", L"Menu", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 2, 2, 260, 215, hWnd, NULL, NULL, NULL);

		LPCWSTR buttonLabels[] = {
		L"Camera ON", L"Camera OFF",
		L"Laser ON", L"Laser OFF",
		L"Deposition ON", L"Deposition OFF"
		};

		const int ids[] = {
		ID_BTN_CAMERA_ON, ID_BTN_CAMERA_OFF,
		ID_BTN_LASER_ON, ID_BTN_LASER_OFF,
		ID_BTN_DEPOSITION_ON, ID_BTN_DEPOSITION_OFF
		};

		for (int i = 0; i < 6; i++) {
			CreateWindowW(L"BUTTON", buttonLabels[i], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15 + (buttonSpacing * i), buttonWidth, buttonHeight, hFrame, (HMENU)ids[i], NULL, NULL);
		}

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

		HWND hCombo = CreateWindowW(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,10, 200, 240, 200, info, (HMENU)ID_CAMERA_OPTION, NULL, NULL);
		// Add items to the combo box
		SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Option 1");
		SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Option 2");
		SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Option 3");
		// Select the default item
		SendMessageW(hCombo, CB_SETCURSEL, 0, 0);
		graphframe = CreateWindowW(L"BUTTON", L"Graph", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 262, 215, 520, 215, hWnd, NULL, NULL, NULL);

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
				stopGraphUpdate = false; // Start updating the graph
				std::thread graphThread(UpdateGraph, graphframe);
				graphThread.detach(); // Detach the graph thread to run independently
				cam.DisplayCameraFrame(g_hFrame1, zoomfram, calFrame);				
				InvalidateRect(hWnd, NULL, TRUE);
			}
				break;
			case ID_BTN_CAMERA_OFF:
			{
				cam.setStopCamera(true);
				stopCamera = true;
				stopGraphUpdate = true; // Stop updating the graph
				Dp.setStopDeposition(true);
				Dp.stop();
				
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
			case ID_BTN_DEPOSITION_ON:
			{
				isDeposition = true;
				mThread.addTask(depositionFunction);
				mThread.start();
			}
			break;
			case ID_BTN_DEPOSITION_OFF:
			{
				isDeposition = false;
				mThread.addTask([] {});
				mThread.stop();
			}
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
