#include<bits/stdc++.h>
using namespace std;
void trimLeadingSpace(vector<string> &token,int &i){
    while(token[1][i]==' '){
        //skip starting space
        i++;
    }
}

void handle_echo_operation(vector<string> token) {
    string result;
    int i = 0;
    bool doubleQuotesStart=false,doubleQuotesEnd=true;
    if(token.size()==1){
        cout<<endl;
        return;
    }

    trimLeadingSpace(token,i);

    while (i < token[1].size()) {
        if (token[1][i] == '\"') {
            if(doubleQuotesStart==false){
                doubleQuotesStart=true;
                doubleQuotesEnd=false;
            }
            else{
                doubleQuotesStart=false;
                doubleQuotesEnd=true;
            }
            i++;
            continue;
        }

        if (doubleQuotesStart) {
            // Preserve spaces within quotes
            result.push_back(token[1][i]);
        } 


        else {
            // Trim multiple spaces outside of quotes
            if (token[1][i] != ' '){
                result.push_back(token[1][i]);
            }

            else if((result.back() != ' ' && !result.empty())) {
                //if string is not empty and i want to add space betwee two words
                 result.push_back(token[1][i]);
            }
        }

        i++;
    }

    if(doubleQuotesEnd==false){
        cout<<"Please close the double quotes"<<endl;
        return;
    }
    else{
    cout << result << endl;
    return;
    }
}