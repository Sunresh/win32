#include "PlotGraph.h"

void PlotGraph::completeOfGraph(HWND pztGraphframe, std::deque<double> lineData,  HWND bdtext, double maxVertical) {
	HDC graphf = GetDC(pztGraphframe);
	RECT rect;
	GetClientRect(pztGraphframe, &rect);
	int padding = 2;
	rect.bottom = rect.bottom - padding;
	rect.right = rect.right - padding;
	rect.left = rect.left + padding;
	rect.top = rect.top + 9 * padding;

	FillRect(graphf, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	double scaleY = 1.5;
	int startIndex = 0;
	int visibleDataPoints = rect.right;
	int endIndex = min(startIndex + visibleDataPoints, static_cast<int>(lineData.size()));

	if (lineData.size() >= static_cast<size_t>(visibleDataPoints - startIndex)) {
		lineData.erase(lineData.begin(), lineData.begin() + (lineData.size() - (visibleDataPoints - startIndex)));
	}
	for (int i = startIndex; i < endIndex - 1; ++i) {
		if (bdtext != NULL) {
			std::wstring newText = L" : " + std::to_wstring(lineData[i]);
			SetWindowTextW(bdtext, newText.c_str());
		}
		double y1 = lineData[i];
		double y2 = lineData[i + 1];
		double normalizedX1 = static_cast<double>(i - startIndex) / (visibleDataPoints - 1);
		double normalizedX2 = static_cast<double>(i + 1 - startIndex) / (visibleDataPoints - 1);

		int startX = static_cast<int>(normalizedX1 * rect.right);
		int endX = static_cast<int>(normalizedX2 * rect.right);

		// Scale the heights based on the available vertical space
		int startY = rect.bottom - static_cast<int>(y1 * scaleY * rect.bottom / maxVertical) - padding;
		int endY = rect.bottom - static_cast<int>(y2 * scaleY * rect.bottom / maxVertical) - padding;

		// Clip the coordinates to ensure they are within the visible area
		startX = std::max<int>(0, std::min<int>(rect.right, startX));
		endX = std::max<int>(0, std::min<int>(rect.right, endX));
		startY = std::max<int>(0, std::min<int>(rect.bottom, startY));
		endY = std::max<int>(0, std::min<int>(rect.bottom, endY));

		// Draw the line only if it's within the visible area
		if (startX < rect.right && endX > 0 && startY < rect.bottom && endY > 0) {
			MoveToEx(graphf, startX, startY, NULL);
			LineTo(graphf, endX, endY);
		}
	}

	startIndex++;
	ReleaseDC(pztGraphframe, graphf);
}

