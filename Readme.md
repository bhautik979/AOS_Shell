How to run the programm:
-> in terminal change control to project folder.
->now write g++ -o shell shell.cpp,all executable file.
->now write./shell


Command:
1)shell: this is the main function of the Assignment. in shell first we will gatting system name and user name and start it printing on terminal.
->after we take infinite input of command by while loop and go to perform operation finction which match the function and five output


2)CD function:
-> cd function will used to change directory, our logical root directory is execulable folder, if we write cd .. then it will goes in parent directory.
cd . will lead in current directory
cd path/folder name will lead to that folder

3)PWD function:
->pwd function will used to print present working directory.
->this function will use getcwd() for getting current directory.

4)echo function
->echo function will trim the all unnecesury spaces which are out of double qoutes.
->echo function print as it is line between double quotes.


5)ls
->ls command use to print information of directory and files
● ls list directory and files
● ls -a list all file including hidden file
● ls -l list in detail
● ls . list in current directory
● ls ..  list in parent directory
● ls ~  list in root directory
● ls -a -l  list all in large
● ls -la / ls -al  list all in large
● ls <Directory/File_name>   provide directory and path
● ls -<flags> <Directory/File_name>  provide directory and path

6)search
->this function is used to search below to current directory
->this will be implemented by Depth first search

7)history
->this command give history command
->this will take one number as argument

8)pinfo
->this command give process information which is passes to it as number
->if we did not pass any value to the pinfo then it will print information of terminal

9)handle_new_process()
->this function is used for creating forground and background process.