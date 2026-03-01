#include <iostream>
#include <string>
#include <sstream>
using namespace std;
int main() {
    std::string line;
    std::string key;

    while (getline(cin, line)) 
    {
        if (line == "QUIT") break;
        if (line.empty()) continue;
        istringstream iss(line);
        string command;
        iss >> command;
        if (command == "PASS") {
            iss >> key;
            cout << "OK\n";
        }
    }

    return 0;
}