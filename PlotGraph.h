#ifndef PLOT_GRAPH_H
#define PLOT_GRAPH_H

#include <algorithm>
#include <Windows.h>
#include <deque>
#include <string>
#include <atomic>

class PlotGraph {
public:

	void completeOfGraph(HWND pztGraphframe, std::deque<double> lineData, double maxVertical);

};

#endif // !PLOT_GRAPH_H
