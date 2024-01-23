#include "ExportCsv.h"

void ExportCSV::saveCSV(std::deque<double> data, std::deque<double> before, const char* fileName) {
	std::string filename_ = std::string(fileName) +".csv";
	std::ofstream file(filename_);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename_ << std::endl;
        return;
    }
    file << "time(s)," << "BD," << "PZT," << std::endl;
    for (size_t i = 0; i < data.size(); i++) {
        // Assuming you want time in seconds with one decimal place
        file << static_cast<double>(i) / 10.0 << "," << data[i] << "," << before[i] * 12.0 << std::endl;
    }

    file.close();
}