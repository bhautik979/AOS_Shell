#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>   // For opendir, readdir, closedir
#include <sys/stat.h> // For stat
#include <cstring>    // For strcmp
#include<bits/stdc++.h>
using namespace std;

// Recursive function to search for a file or folder
bool search_recursive(string currentDirectory,string target) {
    DIR *dir;
    struct dirent *searchCommandFileOrFolder;
    struct stat statbuf;

    if ((dir = opendir(currentDirectory.c_str())) != nullptr) {
    }
    else{
        perror("Not able to open directory in search command");
        return false;
    }


    while ((searchCommandFileOrFolder = readdir(dir)) != nullptr) {  //one by one check all nodes

        string fullPath = currentDirectory + "/" + searchCommandFileOrFolder->d_name;
        string temp=searchCommandFileOrFolder->d_name;
        int i=0;
        
        //if i found node is there then return true;
        while(i<target.length()  &&  target[i]==temp[i]){i++;}   //check all character one by one
        if(i==target.length()){
            closedir(dir);
            return true;
        }

        else{
             //if i found current pointer is at directory then go and do recursive search on there directory

             //skip . file
             if(temp=="."){
                continue;
             }

                //skip .. file
             if(temp==".."){
                continue;
             }

        struct stat buffer;
        int flag=stat(fullPath.c_str(),&buffer);
        if(flag==-1){
            perror("not getting stat in Search_Recursive");
            continue;
        }
         else{
            if(S_ISDIR(buffer.st_mode)){          //i know it is directory
                bool ans=search_recursive(currentDirectory+'/'+temp,target);
                if(ans==true){return true;}
                else{continue;}
            }
            else{
                continue;
            }
         }   
        }
    }

    closedir(dir);
    return false;
}


// handle_search_command function
void handle_search_command(const vector<string>& token) {
    if (token.size() != 2) {
        cout << "Invalid arguments for show command" << endl;
        return;
    }
    int i=0;
    while(i<token[1].size()){
        if(token[1][i]=='/'){
            cout<<"Please provide only file name"<<endl;
            return;
        }
        i++;
    }
    string target = token[1];   //get target file name
    char cwd[1024];


    //check if it is current working dierctory 
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        perror("didn't get current working directory in search function");
        return;
    }
    
    string currentDir = string(cwd);
    
    // Perform the recursive search
    bool ans=search_recursive(currentDir, target);
    if (ans==true) {
        cout << "True" << endl;
    } else {
        cout << "False" << endl;
    }

}
