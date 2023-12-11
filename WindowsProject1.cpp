#include "framework.h"
#include "WindowsProject1.h"
#include "opencv2/opencv.hpp"
#include "camera.cpp"
#include "daqSignal.h"
MyDaq daq;
#define MAX_LOADSTRING 100
#define ID_BTN_CAMERA_ON 130
#define ID_BTN_CAMERA_OFF 131
#define ID_BTN_LASER_ON 132
#define ID_BTN_LASER_OFF 133
bool stopCamera = true;
double meancv;
cv::VideoCapture cap;
std::deque<double> brightData;
Camera cam;
HWND g_hFrame1;
HWND zoomfram;
HWND graphframe;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void DisplayCameraFrame(HWND hWnd,HWND hWn);
void ReleaseCameraResources(HDC hdc);


std::atomic<bool> stopGraphUpdate(true);
void UpdateGraph(HWND graphframe) {
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

			int dataCount = brightData.size();
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
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust the delay as needed
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
		HWND hFrame = CreateWindowW(L"BUTTON", L"Menu", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 2, 2, 260, 215, hWnd, NULL, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Camera ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 15, 255, 30, hFrame, (HMENU)ID_BTN_CAMERA_ON, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Camera OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 47, 255, 30, hFrame, (HMENU)ID_BTN_CAMERA_OFF, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Laser ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 79, 255, 30, hFrame, (HMENU)ID_BTN_LASER_ON, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Laser OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 111, 255, 30, hFrame, (HMENU)ID_BTN_LASER_OFF, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Deposition ON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 143, 255, 30, hFrame, NULL, NULL, NULL);
		CreateWindowW(L"BUTTON", L"Deposition OFF", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 2, 175, 255, 30, hFrame, NULL, NULL, NULL);
		g_hFrame1 = CreateWindowW(L"BUTTON", L"Camera", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 262, 2, 260, 215, hWnd, NULL, NULL, NULL);
		zoomfram = CreateWindowW(L"BUTTON", L"ZOOM", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 522, 2, 260, 215, hWnd, NULL, NULL, NULL);
		graphframe = CreateWindowW(L"BUTTON", L"Graph", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 2, 215, 520, 215, hWnd, NULL, NULL, NULL);
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
				stopCamera = false;
				stopGraphUpdate = false; // Start updating the graph
				std::thread graphThread(UpdateGraph, graphframe);
				graphThread.detach(); // Detach the graph thread to run independently
				DisplayCameraFrame(g_hFrame1, zoomfram);
				//cam.DisplayCameraFrame(g_hFrame1, zoomfram);
				InvalidateRect(hWnd, NULL, TRUE);
			}
				break;
			case ID_BTN_CAMERA_OFF:
			{
				stopCamera = true;
				stopGraphUpdate = true; // Stop updating the graph
				
			}
				break;
			case ID_BTN_LASER_ON:
			{
				daq.digitalOut(nullptr, "Dev2/port0/line0", true);
			}
				break;
			case ID_BTN_LASER_OFF:
			{
				daq.digitalOut(nullptr, "Dev2/port0/line0", false);
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

void DisplayCameraFrame(HWND hWnd, HWND hWn)
{
	cap.open(0);
	if (!cap.isOpened()) {
		return;
	}
	HDC hdc = GetDC(hWnd); // Obtain device context for the window
	HDC hdc1 = GetDC(hWn); // Obtain device context for the window

	cv::Mat dframe,frame;
	while (!stopCamera) {
		cap >> dframe;
		if (!dframe.empty()) {
			cv::flip(dframe, frame, 1);//flip
			cv::resize(frame, frame, cv::Size(260, 215));
			// Show original frame in g_hFrame1
			cv::Mat tmpFrameOriginal;
			cv::cvtColor(frame, tmpFrameOriginal, cv::COLOR_BGR2BGRA);
			// Show cropped portion in zoomfram
			cam.drawRectangle(tmpFrameOriginal,50,50,150,150, cv::Scalar(0, 0, 255),1);
			cv::Mat croppedFrame = frame(cv::Rect(50, 50, 100, 100)); // Example crop - adjust as needed
			cv::Mat tmpFrameCropped;
			cv::cvtColor(croppedFrame, tmpFrameCropped, cv::COLOR_BGR2BGRA);
			cv::resize(tmpFrameCropped, tmpFrameCropped, cv::Size(260, 215));
			meancv = cam.meanofBri(tmpFrameCropped);
			brightData.push_back(meancv);
			std::wstring meanString = std::to_wstring(meancv);
			OutputDebugStringW(meanString.c_str());
			// Display original frame in g_hFrame1
			HBITMAP hBitmapOriginal = CreateBitmap(tmpFrameOriginal.cols, tmpFrameOriginal.rows, 1, 32, tmpFrameOriginal.data);
			HDC memDCOriginal = CreateCompatibleDC(hdc);
			SelectObject(memDCOriginal, hBitmapOriginal);
			BitBlt(hdc, 0, 0, tmpFrameOriginal.cols, tmpFrameOriginal.rows, memDCOriginal, 0, 0, SRCCOPY);
			DeleteDC(memDCOriginal);
			DeleteObject(hBitmapOriginal);

			// Display cropped portion in zoomfram
			HBITMAP hBitmapCropped = CreateBitmap(tmpFrameCropped.cols, tmpFrameCropped.rows, 1, 32, tmpFrameCropped.data);
			HDC memDCCropped = CreateCompatibleDC(hdc1);
			SelectObject(memDCCropped, hBitmapCropped);
			BitBlt(hdc1, 0, 0, tmpFrameCropped.cols, tmpFrameCropped.rows, memDCCropped, 0, 0, SRCCOPY);
			DeleteDC(memDCCropped);
			DeleteObject(hBitmapCropped);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		if (cv::waitKey(1) == 'q') {
			stopCamera = true;
			break;
		}
	}
	ReleaseCameraResources(hdc);
}

void ReleaseCameraResources(HDC hdc) {
	cap.release();
	ReleaseDC(g_hFrame1, hdc);
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

