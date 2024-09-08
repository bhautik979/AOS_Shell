//ls command
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <iomanip>
using namespace std;


//function to get owner permission
void getOwnerPermission(string &permissions,struct stat &fileStat){
    if(fileStat.st_mode & S_IRUSR){
        permissions+='r';
    }
    else{permissions+='-';}


    if(fileStat.st_mode & S_IWUSR){
        permissions+='w';
    }else{
        permissions+='-';
    }

    if(fileStat.st_mode & S_IXUSR){
        permissions+='x';
    }else{
        permissions+='-';
    }
}


//function to get group permission
void getGroupPermission(string &permissions,struct stat &fileStat){
   if(fileStat.st_mode & S_IRGRP){
        permissions+='r';
    }else{
        permissions+='-';
    } 

    
    if(fileStat.st_mode & S_IWGRP) {
        permissions+='w';
    }else{
        permissions+='-';
    }


    if(fileStat.st_mode & S_IXGRP) {
        permissions+='x';
    }else{
        permissions+='-';
    }
}


//function to get other permission
void getOtherPermission(string &permissions,struct stat &fileStat){
    if(fileStat.st_mode & S_IROTH){
        permissions+='r';
    }else{
        permissions+='-';
    }


    if(fileStat.st_mode & S_IWOTH) {
        permissions+='w';
    }else{
        permissions+='-';
    }


    if(fileStat.st_mode & S_IXOTH){
        permissions+='x';
    }else{
        permissions+='-';
    }
}



// Function to get file permissions
string getPermissionsforfile(struct stat &fileStat) {
    string permissions;

    if(S_ISDIR(fileStat.st_mode))
      {permissions+='d';}
    else
      {permissions+='-';}



    getOwnerPermission(permissions,fileStat);
    getGroupPermission(permissions,fileStat);
    getOtherPermission(permissions,fileStat);

    return permissions;
}



//get all details for printing
void getpermission(struct stat fileStat,string &permissions,string &owner,string &group,string &time){
    permissions = getPermissionsforfile(fileStat);
    owner = getpwuid(fileStat.st_uid)->pw_name;
    group = getgrgid(fileStat.st_gid)->gr_name;
    time = string(ctime(&fileStat.st_mtime)).substr(4, 12);
}





// Function to print the details for the '-l' flag
void printLongFormat(struct dirent *directorypointer, string &directory) {
    struct stat fileStat;
    string filePath = directory + "/" + directorypointer->d_name;
    if (stat(filePath.c_str(), &fileStat) < 0) {
        perror("Can not get information about file or directory by stat function");
        return;
    }
    
    string permissions,owner,group,time;
    getpermission(fileStat,permissions,owner,group,time);

    printf("%s %lu %s %s %8ld %s %s\n",permissions.c_str(),fileStat.st_nlink,owner.c_str(),group.c_str(),fileStat.st_size,time.c_str(),directorypointer->d_name);
}



//get permission of file
void printDetailsForfile(struct stat &fileStat,string &filePath) {
    string permissions, owner, group, time;
    getpermission(fileStat, permissions, owner, group, time);

    printf("%s %lu %s %s %8ld %s %s\n",permissions.c_str(),fileStat.st_nlink,owner.c_str(),group.c_str(),fileStat.st_size,time.c_str(),filePath.c_str());
}


//print details after understending input
void printDetails(string lsCommandDir,int showAll,int longFormat,DIR *dir){
    struct dirent *directorypointer;
    int total_Content;
    while ((directorypointer = readdir(dir)) != nullptr) {

        // Skip hidden files unless -a is specified

        if(showAll==true && longFormat==true){
            printLongFormat(directorypointer,lsCommandDir);
        }
        if(showAll==true && longFormat==false){
            cout<<directorypointer->d_name<<endl;
        }
        if(showAll==false && longFormat==true){
            if(directorypointer->d_name[0]=='.'){continue;}
            else{
                printLongFormat(directorypointer,lsCommandDir);
            }
        }
        if(showAll==false && longFormat==false){
            if(directorypointer->d_name[0]=='.'){continue;}
            else{
                cout<<directorypointer->d_name<<endl;
            }
        }
    }

    closedir(dir);
}


void makeAll(bool &showAll){
    showAll=true;
}

void makelong(bool &longFormat){
    longFormat=true;
}

// Function to handle ls command               need to update
void handle_ls_command(vector<string> &token,char logicalRootDir[1024]) {
    string lsCommandDir=".";    //This is for opening perticular directory
    bool showAll = false;
    bool longFormat = false;
    char *currentDir;
    if (getcwd(currentDir, 1024) == nullptr) {
        perror("Error getting current directory");
        return;// 0;
    }

    // Parse tokens
    for (int i = 1; i < token.size(); ++i) {
        if (token[i] == "-a") {
            makeAll(showAll);
        } 
        else if (token[i] == "-l") {
            makelong(longFormat);
        } 
        else if (token[i] == "-la" || token[i] == "-al") {
            makeAll(showAll);
            makelong(longFormat);
        } 
        else if(token[i]=="~"){
            lsCommandDir=logicalRootDir;
        }
        else if (token[i] == "."){
            lsCommandDir = token[i];
            }
        else if(token[i] == ".."){
            lsCommandDir = token[i];
            }
        else if(token[i][0] != '-') {
            lsCommandDir = token[i];
        } 
        else {
            cout << "please give valid aargument for ls command " << token[i] << " is wrong argument" << endl;
            return;
        }
    }



    //if it is a file()
    struct stat fileStat;
    if (stat(lsCommandDir.c_str(), &fileStat) < 0) {
        perror("Error getting information about the file or directory");
        return;
    }
    
    // Check if it's a file
    if (S_ISREG(fileStat.st_mode)) {
        if(longFormat){
        printDetailsForfile(fileStat,lsCommandDir);}
        else{
            cout<<lsCommandDir<<endl;
        }
        return;
    }




    // Open the directory
    DIR *dir = opendir(lsCommandDir.c_str());           //open dir command return directory stream which contain data of all subdirectory and files
    if (dir == nullptr) {                               //if directory is not get opened
        perror("Error to opening directory in ls command");
        return;
    }
    

    //yaha se samaj ke change karna he............
    printDetails(lsCommandDir,showAll,longFormat,dir);
}