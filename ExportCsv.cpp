#include "ExportCsv.h"

void ExportCSV::saveCSV(std::deque<double> data, std::deque<double> before, std::string fileName) {
    std::string folderName = pref.getprefString(CURRENT_FOLDER);
    std::string filename_ = folderName + "\\" + fileName + ".csv";
    std::ofstream file(filename_);

    if (!file.is_open()) {
        return;
    }
    file << "time(s)," << "BD," << "PZT," << std::endl;
    for (size_t i = 0; i < data.size(); i++) {
        // Assuming you want time in seconds with one decimal place
        file << static_cast<double>(i) / 10.0 << "," << data[i] << "," << before[i] << std::endl;
    }

    file.close();
}