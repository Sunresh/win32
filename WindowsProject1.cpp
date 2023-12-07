#include "framework.h"
#include "WindowsProject1.h"
#include "opencv2/opencv.hpp"

#define MAX_LOADSTRING 100
#define ID_BTN_CAMERA_ON 130
#define ID_BTN_CAMERA_OFF 131
bool stopCamera = false;
cv::VideoCapture cap;

HWND g_hFrame1;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void DisplayCameraFrame(HWND hWnd);
void ReleaseCameraResources(HDC hdc);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
		HWND hFrame = CreateWindowW(L"BUTTON", L"Menu", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 2, 2, 260, 215, hWnd, NULL, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Camera ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15, 255, 30, hFrame, (HMENU)ID_BTN_CAMERA_ON, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Camera OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 47, 255, 30, hFrame, (HMENU)ID_BTN_CAMERA_OFF, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Laser ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 79, 255, 30, hFrame, NULL, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Laser OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 111, 255, 30, hFrame, NULL, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Deposition ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 143, 255, 30, hFrame, NULL, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Deposition OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 175, 255, 30, hFrame, NULL, NULL, NULL);
		g_hFrame1 = CreateWindowW(L"BUTTON", L"Camera", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 262, 2, 260, 215, hWnd, NULL, NULL, NULL);
		SetWindowLongPtr(hFrame, GWLP_WNDPROC, (LONG_PTR)WndProc);
	}
	break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case ID_BTN_CAMERA_ON:
				stopCamera = false;
				DisplayCameraFrame(g_hFrame1);
				break;
			case ID_BTN_CAMERA_OFF:
				stopCamera = true;
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

void DisplayCameraFrame(HWND hWnd)
{
	cap.open(0);
	if (!cap.isOpened()) {
		return;
	}
	HDC hdc = GetDC(hWnd); // Obtain device context for the window
	cv::Mat frame;
	while (!stopCamera) {
		cap >> frame;
		if (!frame.empty()) {
			cv::resize(frame, frame, cv::Size(260, 215));
			cv::Mat tmpFrame;
			cv::cvtColor(frame, tmpFrame, cv::COLOR_BGR2BGRA);
			HBITMAP hBitmap = CreateBitmap(tmpFrame.cols, tmpFrame.rows, 1, 32, tmpFrame.data);
			HDC memDC = CreateCompatibleDC(hdc);
			SelectObject(memDC, hBitmap);
			BITMAP bm;
			GetObject(hBitmap, sizeof(bm), &bm);
			BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
			DeleteDC(memDC);
			DeleteObject(hBitmap);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		if (cv::waitKey(0) == 'q') {
			stopCamera = true;
			break;
		}
	}

	ReleaseCameraResources(hdc); // Release the device context
}
void ReleaseCameraResources(HDC hdc) {
	cap.release(); // Release VideoCapture object
	ReleaseDC(g_hFrame1, hdc); // Release the device context
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

