#ifndef LS_COMMAND_H
#define LS_COMMAND_H

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <cstdio>   // For printf
#include <iomanip>  // For formatting output
#include <cstring>  // For string operations

using namespace std;

// Function to get owner permission
void getOwnerPermission(string &permissions, struct stat &fileStat);

// Function to get group permission
void getGroupPermission(string &permissions, struct stat &fileStat);

// Function to get other permission
void getOtherPermission(string &permissions, struct stat &fileStat);

// Function to get file permissions in string format
string getPermissionsforfile(struct stat &fileStat);

// Function to gather all details for file printing (permissions, owner, group, time)
void getpermission(struct stat fileStat, string &permissions, string &owner, string &group, string &time);

// Function to print file details in long format (for '-l' flag)
void printLongFormat(struct dirent *directorypointer, string &directory);

// Function to print details for a single file
void printDetailsForfile(struct stat &fileStat, string &filePath);

// Function to handle and print directory contents based on flags (showAll, longFormat)
void printDetails(string lsCommandDir, int showAll, int longFormat, DIR *dir);

// Functions to set flag values
void makeAll(bool &showAll);
void makelong(bool &longFormat);

// Function to handle the 'ls' command and its variations
void handle_ls_command(vector<string> &token, char logicalRootDir[1024]);

#endif // LS_COMMAND_H
