#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>

using namespace std;

// Function to prepare command arguments for execvp
vector<char*> prepareCommandArgs(const vector<string>& tokens);

// Function to create a new process and execute a command using execvp
void createProcess(char* command, vector<char*> args);

// Function to execute a background process
void execute_background_process(vector<string>& tokens, int processid, vector<pid_t> &background_processes);

// Function to execute a foreground process and wait for it to finish
void execute_forground_process(vector<string>& tokens, int processid, pid_t forground_proc);

// Function to handle new processes (foreground or background) based on tokens
void handle_new_process(vector<string>& tokens, pid_t &forground_proc, vector<pid_t> &background_processes);

#endif // PROCESS_HANDLER_H
