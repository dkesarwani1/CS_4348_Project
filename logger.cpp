#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

int main(int argc, char* argv[]) {
    // Check for correct usage
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <logfile>\n";
        return 1;
    }

    // Open file in append mode
    std::ofstream logFile(argv[1], std::ios::app);
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
        std::string message;
        std::getline(iss, message);
        if (!message.empty() && message[0] == ' ')
            message.erase(0, 1);
        std::time_t now = std::time(nullptr);
        std::tm local_tm = *std::localtime(&now);
        std::ostringstream ts;
        ts << std::put_time(&local_tm, "%Y-%m-%d %H:%M");
        logFile << ts.str() << " [" << action << "]";

        if (!message.empty())
            logFile << " " << message;

        logFile << '\n';
        logFile.flush();
    }
    return 0;
}