#pragma once
#include "framework.h"
#include "WindowsProject1.h"
#include "preferencemanager.h"

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

