#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <logfile>\n";
        return 1;
    }

    ofstream logFile(argv[1], std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: could not open log file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(std::cin, line)) 
    {
        if (line == "QUIT") break;
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string action;
        iss >> action;
        if (action.empty()) continue;

        string message;
        getline(iss, message);
        if (!message.empty() && message[0] == ' ')
            message.erase(0, 1);
        logFile << line << std::endl;
        logFile.flush(); // Ensure line is written immediately
    }

    return 0;
}