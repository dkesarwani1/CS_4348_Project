#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <logfile>\n";
        return 1;
    }
    const char* logfile = argv[1];
    int logPipe[2];
    if (pipe(logPipe) == -1) 
    {
        perror("pipe");
        return 1;
    }
    pid_t logPid = fork();
    if (logPid < 0) 
    {
        perror("fork");
        return 1;
    }

    if (logPid == 0) 
    {
        // child: logger
        dup2(logPipe[0], STDIN_FILENO);

        close(logPipe[0]);
        close(logPipe[1]);

        execl("./logger", "logger", logfile, (char*)nullptr);
        perror("execl logger");
        _exit(1);
    }

    // parent
    close(logPipe[0]); 

    const char* quitMsg = "QUIT\n";
    write(logPipe[1], quitMsg, 5);
    close(logPipe[1]);

    waitpid(logPid, nullptr, 0);
    return 0;
}