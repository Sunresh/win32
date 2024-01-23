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
	ExportCSV() {}
	void saveCSV(std::deque<double> data, std::deque<double> before, const char* fileName);
};


#endif // !EXPORT_CSV_H
