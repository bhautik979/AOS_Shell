#include<bits/stdc++.h>
#include <cstdio> // For fopen, fclose, fwrite, etc.
#include <fcntl.h>  // For open
#include <unistd.h> // For write, close
using namespace std;
void handle_history_command(vector<string> &token,vector<string> &previousCommands){
    if(token.size()>2){cout<<"Please provide valid aargument for history"<<endl;return;}
    if(token.size()==2){ 

        int n=stoi(token[1]);
        if(n>20){cout<<"Please provide nummber less or equal to 20"<<endl;return;}
        if(n<=previousCommands.size()){
            for(int i=previousCommands.size()-n;i<previousCommands.size();i++){
                cout<<previousCommands[i]<<endl;
            }
        }
        
        else{
            for(int i=0;i<previousCommands.size();i++){
                cout<<previousCommands[i]<<endl;
            }
         }
        }

         if(token.size()==1){
        for(int i=0;i<previousCommands.size();i++){
                cout<<previousCommands[i]<<endl;
            }
    }
}


void  initializeVector(vector<string> &previousCommands){
       FILE *file;
       char line[256]; 
       file = fopen("history.txt", "r");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }
        char buffer[1024];  // Buffer for reading lines
        while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newline character from the end
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        previousCommands.push_back(string(buffer));
    }

    // Close the file
        fclose(file);

    return;
}

void reflectInhistoryFile(const vector<string> &previousCommands) {
    int fd = open("history.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   if (fd < 0) {
    perror("Error opening file");
    return;
    }
    for (int i=0;i<previousCommands.size();i++) 
    {
        string command_with_newline =previousCommands[i]+"\n";//inserting a new line as we want to print line by line 
        write(fd,command_with_newline.c_str(),command_with_newline.size());   
    }

    close(fd); // Close the file
}

void updateVector(vector<string> &previousCommands,string &command) {
    if (!command.empty()) {
        // Check if the command vector has more than 20 commands
        //cout<<previousCommands.size();
        if (previousCommands.size() >= 20) {
            // Remove the oldest command (first element)
            previousCommands.erase(previousCommands.begin());
        }
        previousCommands.push_back(command);
    }
}