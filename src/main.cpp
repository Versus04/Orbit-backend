#include "crow.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
class user{
    std::string name,email;
    int id;
    int teamid;
    public:
    user() : name(""), email(""), id(0), teamid(-1) {}

    user(std::string name, std::string email ,int id ,int teamid){
        this->name=name;
        this->email=email;
        this->id=id;
        this->teamid=teamid;
    }
    
};
class Team{
    
    int Teamid;
    std::string TeamName;
    std::vector<int>members;
    std::vector<int>notes;
    public:
    Team() : Teamid(0), TeamName("") {}
    Team(int teamid , std::string teamname)
    {
        this->Teamid=teamid;
        this->TeamName=teamname;
    }
    void addMember(int id)
    {
        members.push_back(id);
    }
    
};
class Note{};
std::unordered_map<int , user>userDB;
std::unordered_map<int,Team>teamDB;
int main(){

    crow::SimpleApp app;
    CROW_ROUTE(app,"/createnote")([](){
        return crow::response(501,"Not Implemented Yet");
    });
    CROW_ROUTE(app,"/editnote")([](){
        return crow::response(501,"Not Implemented Yet");});
    CROW_ROUTE(app,"/deletenote")([](){
        
        return crow::response(501,"Not Implemented Yet");
    });
    CROW_ROUTE(app,"/createuser").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if(!body)return crow::response(400,"Invalid");
        if(!body["name"] || !body["email"])return crow::response(400,"Invalid");
        
        std::string name = body["name"].s();
        std::string email = body["email"].s();
        int id = body["id"].i();
        int teamid =body.has("teamid")?body["teamid"].i():-1;
        user u1(name,email,id,teamid);
        userDB[id]=u1;
        if(teamid!=-1){
            if(teamDB.find(teamid)==teamDB.end())return crow::response(400,"Invalid TeamID");
            teamDB[teamid].addMember(id);
        }
        crow::json::wvalue response;
        response["status"]="Sucess";
        response["message"]="User "+name+" created sucessfully";
        response["id"]=id;
        return crow::response(201,response);
    });
    app.port(8080).multithreaded().run();
}