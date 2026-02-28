#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <logfile>\n";
        return 1;
    }

    std::ofstream logFile(argv[1], std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: could not open log file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "QUIT") break;
    }

    return 0;
}