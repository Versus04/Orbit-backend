#include "crow.h"
#include <iostream>
#include <string>
#include <vector>
class user{
    std::string name,email;
    int id;
    int teamid;
    
};
class Team{
    int Teamid;
    std::string TeamName;
    vector<int>members;
    vector<int>notes;

};
class Note{};
int main(){

    crow::SimpleApp app;
    CROW_ROUTE(app,"/createnote")([](){});
    CROW_ROUTE(app,"/editnote")([](){});
    CROW_ROUTE(app,"/deletenote")([](){});
}