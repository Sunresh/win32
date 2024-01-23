#ifndef EXPORT_CSV_H
#define EXPORT_CSV_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <deque>
#include <chrono>

class ExportCSV {
	struct Csvlist {
		std::string nameapp;
	};
public:
	ExportCSV() {} // Default constructor
	void ExportCSV::saveCSV(std::deque<double> data, std::deque<double> before, const char* fileName);
};
#endif