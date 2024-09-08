#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
using namespace std;

vector<char*> prepareCommandArgs(const vector<string>& tokens) {
    vector<char*> args;
    for (const auto& token : tokens) {
        args.push_back(const_cast<char*>(token.c_str()));
    }
    args.push_back(nullptr);  // Null-terminate the argument list
    return args;
}


void createProcess(char* command,vector<char*> args){
    int num=execvp(command, args.data());
    if(num==-1){
        cout<<"Process can not execute by execvp command"<<endl;
     exit(EXIT_FAILURE);
    }
}


void execute_background_process(vector<string>& tokens,int processid,vector<pid_t> &background_processes) {
    if (processid > 0) {
        cout << "Background process PID: " << processid << endl;
        background_processes.push_back(processid);
    } else {
        // In child process
        setpgid(0, 0);  // Set child process as leader of new process group
        vector<char*> args = prepareCommandArgs(tokens);
        createProcess(args[0],args);
    }
}


void execute_forground_process(vector<string>& tokens,int processid,pid_t forground_proc) {
    if (processid > 0) {
        int status;
        waitpid(processid, &status, 0);
        forground_proc=processid;
    } else {
        // In child process
        vector<char*> args = prepareCommandArgs(tokens);
        createProcess(args[0],args);
    }
}


void handle_new_process(vector<string>& tokens,pid_t &forground_proc, vector<pid_t> &background_processes) {
    int backgroundProcess = 0;

    if (!tokens.empty() && tokens.back() == "&") {
        backgroundProcess = 1;
        tokens.pop_back();  // Remove "&" from tokens
    }
    pid_t processid=fork();
    if  (processid < 0) {
        perror("Failed to fork process");
        exit(EXIT_FAILURE);
    }

    if (backgroundProcess) {
        execute_background_process(tokens,processid,background_processes);
    } else {
        execute_forground_process(tokens,processid,forground_proc);
    }
}