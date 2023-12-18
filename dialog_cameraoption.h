#include "framework.h"
#include "WindowsProject1.h"
int g_selectedFrameRate = 0;

INT_PTR CALLBACK CameraOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_CANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		case IDC_APPLY:
		{
			// Determine which frame rate option is selected
			if (IsDlgButtonChecked(hDlg, IDC_FRAME_RATE_30) == BST_CHECKED) {
				g_selectedFrameRate = 30; // Set the global variable to 30 fps
			}
			else if (IsDlgButtonChecked(hDlg, IDC_FRAME_RATE_60) == BST_CHECKED) {
				g_selectedFrameRate = 60; // Set the global variable to 60 fps
			}
			HWND hWndButton = GetDlgItem(hDlg, IDC_CANCEL); // Get the handle of the button
			SetWindowTextW(hWndButton, L"New Button Text");
			//EndDialog(hDlg, LOWORD(wParam));
			//return (INT_PTR)TRUE;
		}
		break;
		}
	}
	return (INT_PTR)FALSE;
}
