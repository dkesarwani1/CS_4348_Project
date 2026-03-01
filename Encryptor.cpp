#include <iostream>
#include <string>

int main() {
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line == "QUIT") break;
    }

    return 0;
}