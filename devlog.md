Febuary 26th 9:40PM
Today I created a github and have plans to get logger.c++ done
logger.c++ purpose is to create a logger program for writing log entiries to a file.
It will take 1 command line arguement and Which is the log file name and it reads log messages from standard input.
it writes log messages in this format: YYYY-MM-DD HH:MM [ACTION] MESSAGE
it stops running when you type [QUIT]
Log file doesnt: show a menu, encrypt anything, interact with user, validate encryption input

Febuary 27 5:51

Thoughts So Far
Starting with the logger because it is the simplest independent program. It reads log messages from stdin, formats them with a timestamp, and appends to a log file. It terminates on "QUIT".

## Plan For This Session
- Implement logger.cpp in C++
- Add argument checking and file open (append)
- Add stdin read loop with QUIT handling
- Parse action/message from each line
- Add timestamp (YYYY-MM-DD HH:MM)
- Write log entry in required format and test