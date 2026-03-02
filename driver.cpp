#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
using namespace std;

static bool writeLine(int fd, const string& s) 
{
    string out = s;
    if (out.empty() || out.back() != '\n') out.push_back('\n');
    size_t total = 0;
    while (total < out.size()) {
        ssize_t n = write(fd, out.data() + total, out.size() - total);
        if (n < 0) {
            if (errno == EINTR) continue;
            return false;
        }
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
        if (n == 0) return !line.empty();          // EOF
        if (n < 0) 
        {
            if (errno == EINTR) continue;
            return false;
        }
        if (ch == '\n') return true;
        line.push_back(ch);
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

    // logger
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
    close(logPipe[0]);

    // encryptor
    int encIn[2], encOut[2];
    if (pipe(encIn) == -1) { perror("pipe"); return 1; }
    if (pipe(encOut) == -1) { perror("pipe"); return 1; }

    pid_t encPid = fork();
    if (encPid < 0) { perror("fork"); return 1; }
    if (encPid == 0) {
        dup2(encIn[0], STDIN_FILENO);
        dup2(encOut[1], STDOUT_FILENO);
        close(encIn[0]); close(encIn[1]);
        close(encOut[0]); close(encOut[1]);
        execl("./encryptor", "encryptor", (char*)nullptr);
        perror("execl encryptor");
        _exit(1);
    }
    close(encIn[0]);   // parent writes
    close(encOut[1]);  // parent reads

    
    writeLine(encIn[1], "PASS KEY");
    string resp;
    readLine(encOut[0], resp);
    cout << "Encryptor said: " << resp << "\n";

    writeLine(encIn[1], "QUIT");
    writeLine(logPipe[1], "QUIT");
    close(encIn[1]);
    close(encOut[0]);
    close(logPipe[1]);

    waitpid(encPid, nullptr, 0);
    waitpid(logPid, nullptr, 0);
    return 0;
}