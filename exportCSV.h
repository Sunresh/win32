#pragma once
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

	void saveCSV(std::deque<double> data, double before) {
		std::string filename = "pathawithname.csv";
		std::ofstream file(filename);
		file << "time(s)" << "," << "maya"<< std::endl;
		for (int i = 0; i < data.size(); i++) {
			file << before << "," << data[i]<< std::endl;
		}
		file.close();
	}
};
