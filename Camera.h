#include "opencv2/opencv.hpp"
#include "resource.h"
#include "framework.h"

class Camera {
private:
	HWND hWnd;
	HINSTANCE hInstance;

public:
	Camera(HWND hWnd, HINSTANCE hInstance) : hWnd(hWnd), hInstance(hInstance) {}

	void createWindow(const WCHAR* windowClass, const WCHAR* windowTitle, int nCmdShow) {
		hWnd = CreateWindowW(windowClass, windowTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

		if (!hWnd) {
			MessageBoxW(nullptr, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
			return;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
	}

	void startoo() {
		HWND hStaticText = CreateWindowW(L"STATIC", L"Your text goes here...",
			WS_VISIBLE | WS_CHILD, 10, 10, 300, 50, hWnd, nullptr, hInstance, nullptr);
		if (!hStaticText) {
			return;
		}

		cv::Mat image = cv::imread("C:\\Users\\nares\\Source\\Repos\\win32\\Screenshot 2023-11-12 161018.png");
		if (image.empty()) {
			MessageBoxW(hWnd, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
			return;
		}

		// Resize the image to fit within the main window
		cv::Mat resizedImage;
		cv::resize(image, resizedImage, cv::Size(400, 300), 0, 0, cv::INTER_AREA);

		// Convert OpenCV Mat to GDI bitmap
		HBITMAP hBitmap = MatToBitmap(resizedImage);

		// Create a static control to display the image
		HWND hStaticImage = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_BITMAP,
			320, 10, 400, 300, hWnd, nullptr, hInstance, nullptr);

		// Set the bitmap to the static control
		SendMessage(hStaticImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}
	void start() {
        HWND hStaticText = CreateWindowW(L"STATIC", L"Your text goes here...",
            WS_VISIBLE | WS_CHILD, 10, 10, 300, 50, hWnd, nullptr, hInstance, nullptr);
        if (!hStaticText) {
            return;
        }

        cv::VideoCapture cap(0); // Open the default camera (change 0 to another number if you have multiple cameras)

        if (!cap.isOpened()) {
            MessageBoxW(hWnd, L"Failed to open camera", L"Error", MB_OK | MB_ICONERROR);
            return;
        }

        cv::Mat frame;
        cv::namedWindow("Camera Feed", cv::WINDOW_NORMAL);

        while (true) {
            cap >> frame; // Capture frame from camera

            if (frame.empty()) {
                MessageBoxW(hWnd, L"Failed to capture frame", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            // Resize the frame to fit within the main window
            cv::resize(frame, frame, cv::Size(400, 300), 0, 0, cv::INTER_AREA);

            // Convert OpenCV Mat to HBITMAP
            HBITMAP hBitmap = CreateBitmap(frame.cols, frame.rows, 1, 24, frame.data);

            // Create a static control to display the camera feed
            HWND hStaticImage = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_BITMAP,
                10, 70, 400, 300, hWnd, nullptr, hInstance, nullptr);

            // Set the bitmap to the static control
            SendMessage(hStaticImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

            cv::imshow("Camera Feed", frame);

            if (cv::waitKey(30) >= 0)
                break;
        }
        cv::destroyAllWindows();
    }

private:
	HBITMAP MatToBitmap(const cv::Mat& image) {
		int width = image.cols;
		int height = image.rows;
		int bytesPerPixel = image.channels() * 8;
		int stride = (width * bytesPerPixel + 31) / 32 * 4; // DWORD alignment
		size_t dataSize = stride * height;

		BITMAPINFOHEADER bmpInfoHeader = { 0 };
		bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfoHeader.biWidth = width;
		bmpInfoHeader.biHeight = -height; // Negative indicates top-down bitmap
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = static_cast<WORD>(bytesPerPixel);
		bmpInfoHeader.biCompression = BI_RGB;
		bmpInfoHeader.biSizeImage = static_cast<DWORD>(dataSize);

		BITMAPINFO bmpInfo = { 0 };
		bmpInfo.bmiHeader = bmpInfoHeader;

		HBITMAP hBitmap = CreateDIBitmap(GetDC(hWnd), &bmpInfoHeader, CBM_INIT, image.data, &bmpInfo, DIB_RGB_COLORS);
		return hBitmap;
	}
};
