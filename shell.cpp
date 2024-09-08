#include <iostream>
#include <unistd.h>    // For gethostname()
#include <sys/utsname.h> // For uname()
#include <cstdlib>     // For getenv()
#include<bits/stdc++.h>
#include <termios.h>
#include "ls.cpp"
#include"search.cpp"
#include"pinfo.cpp"
#include"history.cpp"
#include"newprocess.cpp"
#include"echo.cpp"
using namespace std;

//void handle_ls_command(vector<string> token);
//global variable
int bufferSize=1024;
char logicalRootDir[1024] = "/home/bhautik/Desktop/2024201077_ASSIGNMENT2";  //this is logical root directory
char  priviousDirectory[1024]="/home/bhautik/Desktop/2024201077_ASSIGNMENT2";  //this store previous directory for cd .. and cd -
char currentWorkingdir[1024];


pid_t current_pid = -1; // Global variable to store the PID of the current child process
int historyIndex = -1;            // Index for traversing history


//create termios
struct termios oldt, newt;
//eneble raw mode for reading arrow key
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}


//disable raw mode for after completion of programm
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}



pid_t forground_proc = -1;  // Track the current foreground process
vector<pid_t> background_processes; // Track background jobs
// Signal handler for SIGINT (Ctrl+C)
void sigint_handler(int sig) {
    if (forground_proc != -1) {
        // Send SIGINT to the current child process
        kill(current_pid, SIGINT);
        cout<<" ";
    }
}

//print path
void printPath(){
    char currentDir[PATH_MAX];
    // Get the username by getlogin_r
    char username[256];
    int result=getlogin_r(username,sizeof(username));

    if(result==0){}
    else{
        perror("can not get user name");
    }

    // Get the system name
    struct utsname systemInfo;  //this is structure
    result=uname(&systemInfo);  //here i give value to structure
    if(result==0){}
    else{
         perror("can not get system information");
    }
    
    //print the username and systemname
    char *systemname=(char *)malloc(500);
    strcpy(systemname,username);
    strcat(systemname,"@");
    strcat(systemname,systemInfo.sysname);
    //const char *c=systemname.c_str();

    if (getcwd(currentDir, sizeof(currentDir)) == nullptr) {
        perror("Error getting current directory");
        return;// 0;
    }

    // Check if the current directory starts with the logical root directory
    int flag=0,i=0;
    while(i<strlen(logicalRootDir)){
        if(currentDir[i]!=logicalRootDir[i]){
            flag=1;
            break;
        }
        i++;
    }


    if (flag == 0) {    //if flag is zero then currentdirectory is start with logical root directory
        
        // Build the relative path
        const char* relativePath = currentDir + strlen(logicalRootDir);    //if my current path is logical root then this line 
       
        // Create the prompt string
        printf("%s:~%s>",systemname, relativePath);
        return;// sizeof(systemname)+sizeof(relativePath)+3;
    } else {
        // If not in the logical root directory, display the full path
        printf("%s>", currentDir);
        return;// sizeof(currentDir);
    }

    return;// 0;
}



//create tokens
void tokenizationInput(vector<string> &tokens,string command,string delemeter){
    int token_count = 0;
    char *input = new char[command.length() + 1];
    strcpy(input,command.c_str());
    char *token = strtok(input,delemeter.c_str());
    
    while (token != NULL) {
        tokens.push_back(token);
        token_count++;
        token = strtok(NULL, delemeter.c_str());
    }

}

vector<string> tokenizationInputforEcho(string command){
    int token_count = 0;
    vector<string> temp;
    char *input = new char[command.length() + 1];
    strcpy(input,command.c_str());
    char *token = strtok(input," ");
    
    while (token != NULL) {
        temp.push_back(token);
        token_count++;
        token = strtok(NULL,"");
    }

    return temp;
}


//handle cd command
void handle_cd_operation(vector<string> token,int token_count){

char path[PATH_MAX];
    //when cd get more then one aargument
    if(token.size()>2){cout<<"Invalid arguments for error handling"<<endl; return;}   


    //when we get only cd
    if (token.size() == 1) {
        // No argument provided, change to the user's home directory
        const char *homeDir = logicalRootDir;        //here i have to pass dynamic value;
        if (homeDir != NULL) {
            if (chdir(homeDir) == 0) {//this is success case
            }
        } else {
            perror("Not able to open directory");
        }
    } 

    
    //When we gwt cd <foldername>
    
    else if (token.size() == 2) {

        string targetDir;
          if (getcwd(currentWorkingdir, sizeof(currentWorkingdir)) == nullptr) 
        {
            perror("Error while getting current working dir in cd");
        } 
        

        // 
        if(token[1]==".."){   //go to parent directory
          // Go to the parent directory
            targetDir = string(currentWorkingdir);

            int i = targetDir.find_last_of('/');
            if (i != targetDir.length()-1) {
                targetDir = targetDir.substr(0, i);
            } 
            else {
                targetDir.clear();  // Root directory case
            }
            if (chdir(targetDir.c_str()) == 0) {
                strcpy(priviousDirectory, currentWorkingdir);
            } else {
                perror("Error to change directory in cd..");
            }

            return;

        }
       
        if(token[1]=="."){strcpy(priviousDirectory, currentWorkingdir);return;}

        if(token[1]=="-"){
            targetDir=priviousDirectory;
            if (chdir(targetDir.c_str()) == 0) {
                strcpy(priviousDirectory, currentWorkingdir);
            } else {
                perror("Error to change directory in cd -");
            }
            return;
        }

        if(token[1]=="~"){
            targetDir=logicalRootDir;
            if (chdir(targetDir.c_str()) == 0) {
                strcpy(priviousDirectory, currentWorkingdir);
            } else {
                perror("Error to change directory in cd -");
            }
            return;
        }


        else{
            // Handle absolute and relative paths
            string fullPath;

            if (token[1][0] == '/') {    // /hello/bhautik/Desktop
                // Absolute path
                fullPath = token[1];
            } 

            else {   //relative path
                    fullPath = string(currentWorkingdir) + "/" + token[1];
                
            }

            //change directory
            if (chdir(fullPath.c_str()) == 0) {
                strcpy(priviousDirectory, currentWorkingdir);
            }else {
                cout<<"Hello from cdfile"<<endl;
                 perror("Error to change directory in cd file");
            }
            return;
        }
        
    } 

    return;
}






void handle_pwd_operation(){
    char currentDir[1024];
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("Error getting current directory");
        return;
    }
    cout<<currentDir<<endl;
    return;
}

//perform operation based on input
void performOperation(vector<string> token,int token_count,string command,vector<string> previousCommands){   //command is pass for echo
    if(token_count==0){return;}
    if(token[0]=="cd"){  //error case handle karvana che
        handle_cd_operation(token,token.size()); 
    }

    else if(token[0]=="pwd"){   //complate
        handle_pwd_operation();
    }


    else if(token[0]=="echo"){    //gpt nu che
        vector<string> temp;
        temp=tokenizationInputforEcho(command);
        handle_echo_operation(temp);
    }

    else if(token[0]=="ls"){   //thodu flow change karvano che
       handle_ls_command(token,logicalRootDir);
    }

    else if(token[0]=="search"){     
        handle_search_command(token);
    }

    else if(token[0]=="history"){   //khali history aapie to 20 thay k 10 e jovanu che
        handle_history_command(token,previousCommands);
    }
    else if(token[0]=="pinfo"){   //background & forground baki che
        handle_pinfo_command(token);
    }
    else{
        handle_new_process(token,forground_proc,background_processes);
    }

}


//handle up arrow
void handle_up_arrow(int &historyIndex,string &command,vector<string> &previousCommands){
    if (historyIndex > 0) {
        historyIndex--;
        command = previousCommands[historyIndex];
        cout << "\033[2K\r" << flush;
        printPath();
        cout<<command;
 }
}


//handle down arrow
void handle_down_arrow(int &historyIndex,string &command,vector<string> &previousCommands){
     if (historyIndex < previousCommands.size() - 1) {
        historyIndex++;
        command = previousCommands[historyIndex];
                        
        cout << "\033[2K\r" << flush;
        printPath();
        cout<<command;
        } else if (historyIndex == previousCommands.size() - 1) {
            historyIndex++;
            command.clear();
            cout << "\033[2K\r" << flush;
            printPath();
            cout<<command;
        }
}

//handle back space
void handleBackSpace(string &command){
    if (!command.empty()) {
                command.pop_back();
                cout << "\b \b"; // Remove the last character from the screen
            }
}

// Function to read the command with history navigation
void readCommand(string &command, vector<string> &previousCommands) {
    char ch;
    command.clear();
    int historyIndex = previousCommands.size(); // Start at the end of history
    enableRawMode(); // Enable raw mode for capturing arrow keys and other characters

    while (true) {
        ch = getchar();

        // Handle arrow keys (Up and Down) for navigating through command history
        if (ch == '\033') { // If the first value is ESC  which is ^[
            getchar(); // Skip the [
            ch=getchar(); { // The real value
                if(ch=='A'){ // Up Arrow (Previous command)
                    handle_up_arrow(historyIndex,command,previousCommands);
                    continue;
                }
                if(ch=='B'){ // Down Arrow (Next command)
                   handle_down_arrow(historyIndex,command,previousCommands);
                   continue;
                }
                else{break;}
            }
        } 
        
        else if (ch == '\n') { // Enter key
            if (!command.empty()) {
                updateVector(previousCommands,command);
                historyIndex = previousCommands.size(); // Reset history index
            }
            cout << endl; // Move to the next line after the command
            break;
        } else if (ch == 127) { // Backspace key
            handleBackSpace(command);
        } 
        else {
            command.push_back(ch);
            cout << ch; // Display the typed character
        }
    }

    disableRawMode(); // Disable raw mode after the command is read
}

int main() {
    char cwd[2024];
    getcwd(cwd,sizeof(cwd));
    strcpy(logicalRootDir,cwd);
    signal(SIGINT, sigint_handler);
    //signal(SIGTSTP, sigtstp_handler);
    // Get the username by getlogin_r
    char username[256];
    int result=getlogin_r(username,sizeof(username));

    if(result==0){
    //    cout<<"username:"<<username;
    }
    else{
        perror("can not get user name");
    }
    

    // Get the system name
    struct utsname systemInfo;  //this is structure
    result=uname(&systemInfo);  //here i give value to structure
    if(result==0){       
    }
    else{
         perror("can not get system information");
    }
    
    //readthe command
    string command="";
    


    //create vector for storing command history in session and initialize it
    vector<string> previousCommands;
    initializeVector(previousCommands);
    while (true) {
        
    //     // Print the prompt
    
        printPath();  
        readCommand(command,previousCommands);
        //updateVector(previousCommands,command);
    
    //     // Check if the user wants to exit
        if (command=="exit") {
            break;
        }

         
        // Process the command (for now, just print it)
        vector<string> initialtoken;
        tokenizationInput(initialtoken,command,";");
     
        //if token_count is zero then continue
        if(initialtoken.size()==0){
            continue;
        }
        
        for(int i=0;i<initialtoken.size();i++){
            vector<string> token;
            tokenizationInput(token,initialtoken[i]," ");
            performOperation(token,token.size(),command,previousCommands);
        }
    }
    
    reflectInhistoryFile(previousCommands);
   
    return 0;
}
