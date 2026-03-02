#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;
int main(int argc, char* argv[]) {
    // Check for correct usage
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <logfile>\n";
        return 1;
    }

    // Open file in append mode
    ofstream logFile(argv[1], ios::app);
    if (!logFile.is_open()) {
        cerr << "Error: could not open log file.\n";
        return 1;
    }

    string line;

    while (getline(cin, line)) 
    {

        if (line == "QUIT") break;
        if (line.empty()) continue;
        istringstream iss(line);
        string action;
        iss >> action;

        if (action.empty()) continue;
        string message;
        getline(iss, message);
        if (!message.empty() && message[0] == ' ')
            message.erase(0, 1);
        time_t now = time(nullptr);
        tm local_tm = *localtime(&now);
        ostringstream ts;
        ts << put_time(&local_tm, "%Y-%m-%d %H:%M");
        logFile << ts.str() << " [" << action << "]";

        if (!message.empty())
            logFile << " " << message;

        logFile << '\n';
        logFile.flush();
    }
    return 0;
}