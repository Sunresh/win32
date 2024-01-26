#ifndef EXPORT_CSV_H
#define EXPORT_CSV_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <deque>
#include <chrono>
#include "preferencemanager.h"
#include "Resource.h"
class ExportCSV {
	struct Csvlist {
		std::string nameapp;
	};
public:
	PreferenceManager pref;
	void ExportCSV::saveCSV(std::deque<double> data, std::deque<double> before, std::string fileName);
};
#endif