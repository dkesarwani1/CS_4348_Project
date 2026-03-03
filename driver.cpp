#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
using namespace std;
// helpers 
static bool writeLine(int fd, const string& s) 
{
    string out = s;
    if (out.empty() || out.back() != '\n') out.push_back('\n');
    size_t total = 0;
    while (total < out.size()) {
        ssize_t n = write(fd, out.data() + total, out.size() - total);
        if (n < 0) { if (errno == EINTR) continue; return false; }
        total += (size_t)n;
    }
    return true;
}

static bool readLine(int fd, string& line) 
{
    line.clear();
    char ch;
    while (true) 
    {
        ssize_t n = read(fd, &ch, 1);
        if (n == 0) return !line.empty();
        if (n < 0)
        { 
            if (errno == EINTR) continue; return false; 
        }
        if (ch == '\n') return true;
        line.push_back(ch);
    }
}

static bool lettersOnly(const string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) if (!isalpha(c)) return false;
    return true;
}

static void printMenu() {
    cout << "\nCommands:\n"
              << "  password\n"
              << "  encrypt\n"
              << "  decrypt\n"
              << "  history\n"
              << "  quit\n"
              << "> ";
}

static void printHistory(const vector<string>& hist) {
    if (hist.empty()) {
        cout << "(history empty)\n";
        return;
    }
    for (size_t i = 0; i < hist.size(); i++) {
        cout << i + 1 << ") " << hist[i] << "\n";
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <logfile>\n";
        return 1;
    }
    const char* logfile = argv[1];

    // fork logger
    int logPipe[2];
    if (pipe(logPipe) == -1) 
    { 
        perror("pipe"); return 1; 
    }

    pid_t logPid = fork();
    if (logPid < 0) 
    {
         perror("fork"); return 1; 
    }

    if (logPid == 0) 
    {
        dup2(logPipe[0], STDIN_FILENO);
        close(logPipe[0]); close(logPipe[1]);
        execl("./logger", "logger", logfile, (char*)nullptr);
        perror("execl logger");
        _exit(1);
    }
    close(logPipe[0]); // parent writes to logger

    // fork encryptor
    int encIn[2], encOut[2];
    if (pipe(encIn) == -1) 
    { 
        perror("pipe"); return 1; 
    }
    if (pipe(encOut) == -1) 
    {
         perror("pipe"); return 1; 
    }

    pid_t encPid = fork();
    if (encPid < 0) 
    { 
        perror("fork"); return 1; 
    }

    if (encPid == 0) 
    {
        dup2(encIn[0], STDIN_FILENO);
        dup2(encOut[1], STDOUT_FILENO);
        close(encIn[0]); close(encIn[1]);
        close(encOut[0]); close(encOut[1]);
        execl("./encryptor", "encryptor", (char*)nullptr);
        perror("execl encryptor");
        _exit(1);
    }
    close(encIn[0]);   // parent writes commands
    close(encOut[1]);  // parent reads responses

    writeLine(logPipe[1], "START Driver started");

    string cmd;
    string pass;
    bool hasPass = false;

    vector<string> history;

    auto sendEncryptor = [&](const string& line, string& resp) -> bool 
    {
        if (!writeLine(encIn[1], line)) return false;
        return readLine(encOut[0], resp);
    };

    while (true) 
    {
        printMenu();
        if (!getline(cin, cmd)) break;

        if (cmd == "password") {
            cout << "Enter password (letters only): ";
            getline(cin, pass);

            writeLine(logPipe[1], "CMD password");

            if (!lettersOnly(pass)) {
                cout << "ERROR Password must be letters only\n";
                writeLine(logPipe[1], "RESULT ERROR Password must be letters only");
                continue;
            }

            string resp;
            if (!sendEncryptor("PASS " + pass, resp)) {
                cerr << "Encryptor pipe closed.\n";
                break;
            }
            cout << resp << "\n";
            writeLine(logPipe[1], "RESULT " + resp);

            hasPass = (resp.rfind("RESULT", 0) == 0); // starts with "RESULT"
        }
        else if (cmd == "encrypt") 
        {
            writeLine(logPipe[1], "CMD encrypt");

            if (!hasPass) 
            {
                cout << "ERROR Set password first\n";
                writeLine(logPipe[1], "RESULT ERROR Set password first");
                continue;
            }
            cout << "Enter plaintext (letters only): ";
            string plain;
            getline(cin, plain);
            if (!lettersOnly(plain)) 
            {
                cout << "ERROR Text must be letters only\n";
                writeLine(logPipe[1], "RESULT ERROR Text must be letters only");
                continue;
            }
            string resp;
            if (!sendEncryptor("ENCRYPT " + plain, resp)) 
            {
                cerr << "Encryptor pipe closed.\n";
                break;
            }
            cout << resp << "\n";
            writeLine(logPipe[1], "RESULT " + resp);

            if (resp.rfind("RESULT ", 0) == 0) history.push_back(resp.substr(7));
        }
        else if (cmd == "decrypt") 
        {
            writeLine(logPipe[1], "CMD decrypt");

            if (!hasPass) 
            {
                cout << "ERROR Set password first\n";
                writeLine(logPipe[1], "RESULT ERROR Set password first");
                continue;
            }

            cout << "Enter ciphertext (letters only): ";
            string cipher;
            getline(cin, cipher);

            if (!lettersOnly(cipher)) 
            {
                cout << "ERROR Text must be letters only\n";
                writeLine(logPipe[1], "RESULT ERROR Text must be letters only");
                continue;
            }

            string resp;
            if (!sendEncryptor("DECRYPT " + cipher, resp)) 
            {
                cerr << "Encryptor pipe closed.\n";
                break;
            }
            cout << resp << "\n";
            writeLine(logPipe[1], "RESULT " + resp);

            if (resp.rfind("RESULT ", 0) == 0) history.push_back(resp.substr(7));
        }
        else if (cmd == "history") 
        {
            writeLine(logPipe[1], "CMD history");
            printHistory(history);
        }
        else if (cmd == "quit") 
        {
            writeLine(logPipe[1], "CMD quit");
            break;
        }
        else 
        {
            cout << "Unknown command.\n";
        }
    }
    string resp;
    sendEncryptor("QUIT", resp);

    writeLine(logPipe[1], "QUIT"); //makes logger stop

    close(encIn[1]);
    close(encOut[0]);
    close(logPipe[1]);

    waitpid(encPid, nullptr, 0);
    waitpid(logPid, nullptr, 0);

    return 0;
}