# CS_4348_Project 1

## Overview

This project implements a multi-process encryption system using:

- logger.cpp
- encryptor.cpp
- driver.cpp

The programs communicate using UNIX pipes and demonstrate inter-process communication using:

- fork()
- pipe()
- dup2()
- exec()

---

## Programs

## 1. logger.cpp
- Appends timestamped entries to a log file.
- Format: `YYYY-MM-DD HH:MM [ACTION] MESSAGE`
- Ignores QUIT command.
- Runs until it receives `QUIT`.

Compile: g++ -std=c++17 logger.cpp -o logger
RUN: ./logger logfile.txt
Sends your input to Log every time until you type QUIT

## 2. Encryptor.cpp
Implements Vigenère cipher with the following commands:

- PASS key
- ENCRYPT text
- DECRYPT text
- QUIT

Rules:
- Password must be letters only.
- Text must be letters only.
- ENCRYPT/DECRYPT require password to be set first.

Compile: g++ -std=c++17 Encryptor.cpp -o encryptor
Run: ./encryptor

## 3. driver.cpp
Main interface program that:

- Forks logger and encryptor
- Connects pipes using dup2
- Sends commands to encryptor
- Logs actions/results through logger
- Maintains session history

Compile: g++ -std=c++17 driver.cpp -o driver
Run: ./driver logfile.txt

---

## How It Works

Driver Process:
- Creates pipes
- Forks logger and encryptor
- Uses dup2 to redirect stdin/stdout
- Sends commands to encryptor
- Reads responses
- Logs activity

Encryptor Process:
- Stores password
- Encrypts/decrypts using Vigenère cipher
- Prints RESULT or ERROR messages

Logger Process:
- Writes timestamped log entries
- Stops on QUIT

---

## Example Session
