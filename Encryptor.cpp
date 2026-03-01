#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;
static bool all_letters(const string& s) 
{
    if (s.empty()) return false;
    for (unsigned char ch : s) 
    {
        if (!isalpha(ch)) return false;
    }
    return true;
}

static string to_upper(const string& s) 
{
    string out = s;
    for (char& c : out) c = (char)toupper((unsigned char)c);
    return out;
}
static string vigenere_decrypt(const string& cipher, const string& key) {
    string out;
    out.reserve(cipher.size());
    int a = 0;

    for (size_t x = 0; x < cipher.size(); x++) 
    {
        char c = cipher[x];
        int shift = key[a % (int)key.size()] - 'A';
        char p = (char)('A' + (c - 'A' - shift + 26) % 26);
        out.push_back(p);
        a++;
    }
    return out;
}
static string vigenere_encrypt(const string& plain, const string& key) 
{
    string out;
    out.reserve(plain.size());
    int k = 0;

    for (size_t i = 0; i < plain.size(); i++) 
    {
        char p = plain[i];                 // 'A'..'Z'
        int shift = key[k % (int)key.size()] - 'A';
        char c = (char)('A' + (p - 'A' + shift) % 26);
        out.push_back(c);
        k++;
    }
    return out;
}

int main() 
{
    string line;
    string key;
    bool has_key = false;

    while (getline(cin, line)) 
    {
        if (line == "QUIT") break;
        if (line.empty()) continue;

        istringstream iss(line);
        string command;
        iss >> command;

        // Grab rest of line as argument (may contain spaces)
        string arg;
        getline(iss, arg);
        if (!arg.empty() && arg[0] == ' ') arg.erase(0, 1);

        // Normalize command
        for (char& c : command) c = (char)toupper((unsigned char)c);

        if (command == "PASS") 
        {
            if (arg.empty()) 
            {
                cout << "ERROR Missing passkey\n";
                continue;
            }
            if (!all_letters(arg)) 
            {
                cout << "ERROR Passkey must be letters only\n";
                continue;
            }
            key = to_upper(arg);
            has_key = true;
            cout << "RESULT\n";
        }
        else if (command == "ENCRYPT")
         {
            if (!has_key) 
            {
                cout << "ERROR Password not set\n";
                continue;
            }
            if (arg.empty()) 
            {
                cout << "ERROR Missing text\n";
                continue;
            }
            if (!all_letters(arg)) 
            {
                cout << "ERROR Text must be letters only\n";
                continue;
            }
            string plain = to_upper(arg);
            string cipher = vigenere_encrypt(plain, key);
            cout << "RESULT " << cipher << "\n";
        }
        else if (command == "DECRYPT") 
        {
            if (!has_key) 
            {
                cout << "ERROR Password not set\n";
                continue;
            }
            if (arg.empty()) {
                cout << "ERROR Missing text\n";
                continue;
            }
            if (!all_letters(arg)) {
                cout << "ERROR Text must be letters only\n";
                continue;
            }
            string cipher = to_upper(arg);
            cout << "RESULT " << vigenere_decrypt(cipher, key) << "\n";
        }
        else 
        {
            cout << "ERROR Unknown command\n";
        }
    }

    return 0;
}