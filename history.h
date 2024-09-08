#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>  // For FILE, fopen, fclose
#include <fcntl.h>  // For open
#include <unistd.h> // For write, close
#include <cstring>  // For strlen

using namespace std;

// Function to handle the history command
void handle_history_command(vector<string> &token, vector<string> &previousCommands);

// Function to initialize the vector by loading commands from history.txt
void initializeVector(vector<string> &previousCommands);

// Function to write the commands back into history.txt
void reflectInhistoryFile(const vector<string> &previousCommands);

// Function to update the vector with a new command
void updateVector(vector<string> &previousCommands, string &command);

#endif // HISTORY_H