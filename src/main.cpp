#include "crow.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
int binarysearch(int key , int start , int end ,std::vector<int>& nums)
{
    if (start > end) return -1;
    int mid = start+(end-start)/2;
    if(nums[mid]==key)return mid;
    else if(nums[mid]>key)
    {
        return binarysearch(key,start,mid-1,nums);
    }else if(nums[mid]<key)
    {
        return binarysearch(key,mid+1,end,nums);
    }
}
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
    void changeteam(int idi)
    {
        this->teamid=idi;
    }
    int getteam()
    {
        return teamid;
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
    void addNotes(int id)
    {
        notes.push_back(id);
    }
    int getnote(int noteid)
    {
        return binarysearch(noteid,0,notes.size()-1,notes);
    }

    
};
class Note{
    int noteid;
    int version{0};
    int lastupdatedby{-1};
    int teamid;
    std::string title;
    std::string body;
    public:
    Note(int noteid , int teamid ,std::string title ,std::string body){
        this->noteid = noteid;
        this->teamid=teamid;
        this->title=title;
        this->body=body;
    }
    void updated(int creatorid , std::string newcontent)
    {
        body=newcontent;
        this->version++;
        this->lastupdatedby=creatorid;
    }
    int getteamid()
    {
        return teamid;
    }
    std::string gettitle()
    {
        return title;
    }
    std::string getbody()
    {
        return body;
    }
};
std::unordered_map<int , user>userDB;
std::unordered_map<int,Team>teamDB;
std::unordered_map<int,Note>noteDB;
int main(){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    crow::SimpleApp app;
    CROW_ROUTE(app,"/createnote")([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if(!body)return crow::response(400,"Invalid");
        if(!body.has("creatorid") || !body.has("teamid"))return crow::response(400,"Invalid");
        int creatorid = body["creatorid"].i();
        int teamid = body["teamid"].i();
        if(userDB.find(creatorid)==userDB.end())return crow::response(400,"Invalid");
        if(teamDB.find(teamid)==teamDB.end())return crow::response(400,"Invalid");
        int idih = rand();
        while(noteDB.find(idih)!=noteDB.end())idih=rand();
        int noteid = idih;
        std::string title = body.has("title")?body["title"].s():std::string(" ");
        
        std::string content = body.has("body")?body["body"].s():std::string(" ");
        Note n1(noteid,teamid,title,content);
        teamDB[teamid].addNotes(noteid);
        noteDB[noteid]=n1;
        crow::json::wvalue res;
        res["status"]="Sucess";
        res["Message"]="Note created sucessfully";
        res["noteid"]=noteid;
        return crow::response(201,res);
    });
    CROW_ROUTE(app,"/editnote")([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if(!body)return crow::response(400,"Invalid");
        if(!body.has("noteid") || !body.has("editorid"))return crow::response(400,"Invalid");
        int editorid = body["editorid"].i();
        int noteid = body["noteid"].i();
        if(noteDB.find(noteid)==noteDB.end())return crow::response(400,"Invalid");;
        if(userDB.find(editorid)==userDB.end())return crow::response(400,"Invalid");
        int teamid = userDB[editorid].getteam();
        if(teamDB.find(teamid)==teamDB.end()){
            return crow::response(400,"Invalid");
        }
        if(noteDB[noteid].getteamid()!=teamid)
        {
            return crow::response(400,"Invalid");
        }
        else {
            Note& xd = noteDB[noteid];
            std::string newcontent = body.has("content") ? body["content"].s() : std::string("");
            xd.updated(editorid,newcontent);
            //noteDB[noteid]=xd;
        }
        crow::json::wvalue res;
        res["Status"]="Sucess";
        res["message"]="Note is sucessfully edited";
        return crow::response(201,res);
    });
    CROW_ROUTE(app,"/deletenote").methods("POST"_method)([](const crow::request& req){
        
         auto body = crow::json::load(req.body);
        if(!body)return crow::response(400,"Invalid");
        if(!body.has("noteid") || !body.has("deleterid"))return crow::response(400,"Invalid");
        int deleterid = body["deleterid"].i();
        int noteid = body["noteid"].i();
        if(userDB.find(deleterid)==userDB.end())return crow::response(400,"Invalid");
        if(noteDB.find(noteid)==noteDB.end())return crow::response(400,"Invalid");
        noteDB.erase(noteid);
        crow::json::wvalue res;
        res["status"]="Sucess";
        res["message"]="Sucessfully deleted the note";
        return crow::response(201,res);
        
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
    CROW_ROUTE(app,"/createteam").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if(!body)return crow::response(400,"Invalid");
        if(!body.has("teamname") || !body.has("creatorid"))return crow::response(400,"Invalid");
        int creatorid = body["creatorid"].i();
        if(userDB.find(creatorid)==userDB.end())return crow::response(400,"Invalid");
        
        int idih =rand();
        while(teamDB.find(idih)!=teamDB.end()){
            idih = rand();
        }
        int teamid =idih;
        std::string teamname = body["teamname"].s();
        Team t1(teamid,teamname);
        t1.addMember(creatorid);
        userDB[creatorid].changeteam(teamid);
        teamDB[teamid] =t1;
        crow::json::wvalue resp;
        resp["status"]="Sucess";
        resp["message"] = "Team "+ teamname+" with "+std::to_string(teamid)+" created sucessfully";
        return crow::response(201,resp);
    });
    app.port(8080).multithreaded().run();
}