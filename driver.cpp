#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <logfile>\n";
        return 1;
    }
    const char* logfile = argv[1];

    // Logger pipe
    int logPipe[2];
    if (pipe(logPipe) == -1) { perror("pipe"); return 1; }

    pid_t logPid = fork();
    if (logPid < 0) { perror("fork"); return 1; }

    if (logPid == 0) {
        dup2(logPipe[0], STDIN_FILENO);
        close(logPipe[0]); close(logPipe[1]);
        execl("./logger", "logger", logfile, (char*)nullptr);
        perror("execl logger");
        _exit(1);
    }
    close(logPipe[0]); // parent keeps logPipe[1]

    // Encryptor pipes
    int encIn[2];   
    int encOut[2];  
    if (pipe(encIn) == -1) { perror("pipe"); return 1; }
    if (pipe(encOut) == -1) { perror("pipe"); return 1; }

    pid_t encPid = fork();
    if (encPid < 0) { perror("fork"); return 1; }

    if (encPid == 0) {
        // child encryptor
        dup2(encIn[0], STDIN_FILENO);
        dup2(encOut[1], STDOUT_FILENO);

        close(encIn[0]); close(encIn[1]);
        close(encOut[0]); close(encOut[1]);

        execl("./encryptor", "encryptor", (char*)nullptr);
        perror("execl encryptor");
        _exit(1);
    }

    // parent
    close(encIn[0]);   
    close(encOut[1]); 

    //tell encryptor quit and logger quit, then exit
    write(encIn[1], "QUIT\n", 5);
    close(encIn[1]);

    write(logPipe[1], "QUIT\n", 5);
    close(logPipe[1]);

    waitpid(encPid, nullptr, 0);
    waitpid(logPid, nullptr, 0);
    return 0;
}