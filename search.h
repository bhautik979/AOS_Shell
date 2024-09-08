#ifndef SEARCH_HANDLER_H
#define SEARCH_HANDLER_H

#include <iostream>
#include <vector>
#include <string>

// Function to recursively search for a file or folder
bool search_recursive(const std::string& currentDirectory, const std::string& target);

// Function to handle the 'search' command, which searches for a specified file or folder in the current directory
void handle_search_command(const std::vector<std::string>& token);

#endif // SEARCH_HANDLER_H
