#ifndef PINFO_HANDLER_H
#define PINFO_HANDLER_H

#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

// Function to get process status (State)
bool getStatus(string &tempLine, string &processStatus, int processid);

// Function to get memory (VmSize)
bool getMemory(string &tempLine, string &memory);

// Function to get the executable path
bool getExecutablePath(int pid, string &executablePath);

// Function to get process information from the /proc/<pid>/status file
void getInformation(FILE *f, string &processStatus, string &memory, string &executablePath, int pid);

// Function to print process details (PID, process status, memory, and executable path)
void printDetails(int pid, string processStatus, string memory, string executablePath);

// Function to handle the 'pinfo' command, retrieving information about the current process or a specified process
void handle_pinfo_command(vector<string> token);

#endif // PINFO_HANDLER_H
