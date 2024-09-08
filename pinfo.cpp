#include<bits/stdc++.h>
using namespace std;

// Function to get process status (State)
bool getStatus(string &tempLine, string &processStatus,int processid) {
    if (tempLine.find("State:") == 0) {
        processStatus = tempLine.substr(tempLine.find(':') + 1);
        //processStatus.erase(0, processStatus.find_first_not_of(" \t")); // Remove leading spaces
        if(processStatus.find("(running)")!=string::npos){
            processStatus="R";
        }
        else if(processStatus.find("(sleeping)")!=string::npos){
            processStatus="S";
        }
        else if(processStatus.find("(zombie)")!=string::npos){
            processStatus="Z";

        }
        else{
            processStatus="T";
        }


        // Check if the process is in the foreground
        pid_t pgid = getpgid(processid);     //get groupid of new process
        pid_t terminal_pgid = tcgetpgrp(STDIN_FILENO);    //terminal contain groupid of all processes which are running in forground
        if (pgid == terminal_pgid) {
            processStatus += "+";
        }

        return true;
    }

    return false;
}

// Function to get memory (VmSize)
bool getMemory(string &tempLine, string &memory) {
    if (tempLine.find("VmSize:") == 0) {
        memory = tempLine.substr(tempLine.find(':') + 1);
        //memory.erase(0, memory.find_first_not_of(" \t")); // Remove leading spaces
        return true;
    }
    return false;
}

// Function to get the executable path
bool getExecutablePath(int pid, string &executablePath) {
    string filePath = "/proc/" + to_string(pid) + "/exe";
    char resolvedPath[1024];
    int len = readlink(filePath.c_str(), resolvedPath, sizeof(resolvedPath) - 1);   //return symbolic link
    if (len != -1) {
        resolvedPath[len] = '\0';  //add for termination
        executablePath = string(resolvedPath);
    } else {
        executablePath = "Unknown";
    }
    return true;
}

// Function to get process information
void getInformation(FILE *f, string &processStatus, string &memory, string &executablePath, int pid) {
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f)) {
        string tempLine=string(buffer);

        // Find State
        if (processStatus == " " && getStatus(tempLine, processStatus,pid)) {
        }
        // Find VmSize (Memory)
        else if (memory == " " && getMemory(tempLine, memory)) {
        }
    }
    getExecutablePath(pid, executablePath);
}

// Function to print process details
void printDetails(int pid, string processStatus, string memory, string executablePath) {
    cout << "Pid -- " << pid<<endl;
    cout << "Process Status -- " << processStatus<<endl;
    cout << "Memory -- " << memory;
    cout << "Executable Path -- " << executablePath<<endl;
}

// Function to handle pinfo command
void handle_pinfo_command(vector<string> token) {

    if(token.size()==1){
        pid_t pid = getpid();
        string path = "/proc/" + to_string(pid) + "/status";
         FILE *f = fopen(path.c_str(), "r");
        if (f == NULL) {
            perror("Cannot open file in pinfo command");
            return;
        }

        string processStatus = " ";
        string memory = " ";
        string executablePath = " ";

         // Get the information about process
        getInformation(f, processStatus, memory, executablePath, pid);
        fclose(f);

        printDetails(pid, processStatus, memory, executablePath);
    }

    else if (token.size() == 2) {
        int pid = stoi(token[1]);
        string path = "/proc/" + token[1] + "/status";
        FILE *f = fopen(path.c_str(), "r");
        if (f == NULL) {
            perror("Cannot open file in pinfo command");
            return;
        }

        string processStatus = " ";
        string memory = " ";
        string executablePath = " ";
        
        // Get the information about process
        getInformation(f, processStatus, memory, executablePath, pid);
        fclose(f);

        printDetails(pid, processStatus, memory, executablePath);
    } else {
        cout << "Please enter a valid pinfo operation" << endl;
    }
}
