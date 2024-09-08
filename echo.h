#ifndef ECHO_H
#define ECHO_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Function to trim leading spaces from the token
void trimLeadingSpace(vector<string> &token, int &i);

// Function to handle the echo operation, which processes the echo command
void handle_echo_operation(vector<string> token);

#endif // ECHO_H