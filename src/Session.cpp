
#include "../include/User.h"
#include "../include/Tags.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Tags.h"
#include "../include/json.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <ios>
#include <sstream>

class Episode;
class User;
class Watchable;

using namespace std;
// Constructors
Session::Session(const std::string &configFilePath):
        content() , actionsLog(), userMap(), activeUser(nullptr) {


    using json = nlohmann::json;
    json j;
    ifstream i(configFilePath);
    i >> j;
    long id = 1;
    for (size_t i = 0; i < j["movies"].size(); i++) {
        string name = j["movies"][i]["name"];
        int length = j["movies"][i]["length"];
        vector<string> tags = j["movies"][i]["tags"];
        Movie *m_temp = new Movie(id, name, length, tags);
        content.push_back(m_temp);//need movie: move constructor, rule of 5
        id++;
    }

    for (size_t i = 0; i < j["tv_series"].size(); i++) {
        string name = j["tv_series"][i]["name"];
        int episode_length = j["tv_series"][i]["episode_length"];
        vector<string> tags = j["tv_series"][i]["tags"];
        for (size_t k = 0; k < j["tv_series"][i]["seasons"].size(); k++) {//season
            for (int m = 1; m <= j["tv_series"][i]["seasons"][k]; m++) {//episode
                Episode *e_temp = new Episode(id, name, episode_length, k + 1, m, tags);
                content.push_back(e_temp);
                //need Episode: move constructor, rule of 5
                id++;
            }
        }

    }
         j.clear();

}
//Copy Constructor
Session::Session(const Session &other)
        :content{} , actionsLog{} , userMap{}, activeUser{}
{
    activeUser = other.activeUser->clone();
    for(const Watchable* con : other.content){
        this -> content.push_back(con->clone() );
    }

    for(const BaseAction* ba : other.actionsLog){
        this -> actionsLog.push_back( ba -> clone() );
    }


};

// Move Constructor
Session::Session(Session &&other) : content{other.content.size()} , actionsLog{other.actionsLog.size()} ,
                                    userMap{other.userMap.size()} , activeUser{other.activeUser}
{
    for(unsigned int index = 0; index < this->content.size(); index++) {
        this->content.at(index) = other.content.at(index);
    }
    other.content.clear();

    for(unsigned int index = 0; index < this->actionsLog.size(); index++) {
        this->actionsLog.at(index) = other.actionsLog.at(index);
    }
    other.actionsLog.clear();

    for(std::pair<std::string, User*> element : this->userMap){
        this -> userMap = other.userMap;
    }
    other.userMap.clear();
}

// Copy Assignment Operator
Session& Session::operator=(const Session &other) {
    if(&other != this) {
//actionslog
        for (BaseAction *ba : this->actionsLog) {
            delete ba;
            ba = nullptr;
        }
        this->actionsLog.clear();

        for (const BaseAction *ba : other.actionsLog) {
            this->actionsLog.push_back(ba->clone());
        }

//content
        for (Watchable *wa : this->content) {
            delete wa;
            wa = nullptr;
        }
        this->content.clear();

        for (const Watchable *wa : other.content) {
            this->content.push_back(wa->clone());
        }
//usermap
        for (std::pair<std::string, User *> element : this->userMap) {
            //delete element.first;//todo ask about deleting an unordered map
            element.second = nullptr;
            delete element.second;
        }
        userMap.clear();

        for (const std::pair<std::string, User *> element : other.userMap) {
            this->userMap.insert({ element.first, element.second });
        }

        this->activeUser = other.activeUser;
    }
    return  *this;
}


//move Assignment Operator
Session& Session::operator =(Session &&other){
    if(&other != this){
        for(Watchable* wa : this->content) {
            delete wa;
            wa = nullptr;
        }
        this->content.clear();

        for(BaseAction* ba : this->actionsLog) {
            delete ba;
            ba = nullptr;
        }
        this->actionsLog.clear();

        for(std::pair<std::string, User *> element : this->userMap){
            element.second = nullptr;
            delete element.second;
        }
        this -> userMap.clear();
    }
    return *this;
}

// Destructor
Session::~Session()
{
    for(Watchable* wa : this -> content) {
        delete wa;
        wa= nullptr;
    }
    for(BaseAction* ba : this -> actionsLog) {
        delete ba;
        ba = nullptr;
    }

    for(std::pair<std::string, User *> element : this->userMap){
        element.second = nullptr;
        delete element.second;
    }
};

//Copy
//Methods
User *Session::getActiveUser() {
    return activeUser;
}


std::vector<std::string> split (const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}



void Session::start() {

    std::cout << "SPLFLIX is now on" << std::endl;
    std::cout << "Please Enter An Action To Execute" << std::endl;
    std::string temp = "default";
    User *defUser = new LengthRecommenderUser(temp);
    userMap["default"] = defUser;
    activeUser = defUser;
    std::string input;
    BaseAction *action;
    vector<string> cmd;

    bool go = true;
     while(go)
     {

        getline(std::cin, input);
        cmd = split(input, ' ');


        if (cmd[0] == "createuser") {
            std::string name = cmd[1];
            action = new CreateUser(name, cmd[2]);//

        }

        if (cmd[0] == "changeuser") {
            std::string name = cmd[1];
            action = new ChangeActiveUser(name);

        }

        if (cmd[0] == "deleteuser") {
            std::string name = cmd[1];
            action = new DeleteUser(name);

        }

        if (cmd[0] == "dupuser") {//DuplicateUser
            std::string name1 = cmd[1];
            std::string name2 = cmd[2];
            action = new DuplicateUser(name1, name2);

        }

        if (cmd[0] == "content") {//PrintContentList
            action = new PrintContentList();


        }
        if (cmd[0] == "watchhist") {//PrintWatchHistory
            //prints watch history of the current user
            action = new PrintWatchHistory();

        }
        if (cmd[0] == "watch") {
            std::string::size_type sz;
            long idNum = std::stol(cmd[1], &sz);
            action = new Watch(idNum);

        }
        if (cmd[0] == "log") {
            action = new PrintActionsLog();

        }
        if(cmd[0] == "exit")
            go = false;
        else {

                action->setCommand(input);
                action->act(*this);
                actionsLog.push_back(action);
            }
        }


    }


bool Session::isValidType(std::string type) const {
    return  (type == "len" || type == "rer" || type == "gen");
}

std::vector<Watchable *> Session::getContent() const {
    return this->content;
}

std::vector<BaseAction *> Session::getActionsLog() {
    return std::vector<BaseAction *>(actionsLog);
}

std::unordered_map<std::string, User *> Session::getUserMap() {
    return this->userMap;
}

void Session::SetActiveUser(User* u) {
    this -> activeUser = u;
}

void Session::addUserToMap(std::string key, User *newUser) {

    userMap[key] = newUser;

}

void Session::deleteFromMap(std::string key) {

    this->userMap.erase(key);

}

void Session::addWatchableToUser(Watchable *w) {
    this->userMap[activeUser->getName()]->get_history().push_back(w);


}

void Session::showLog() {
    for(BaseAction* a : actionsLog) {
        std::cout << a->toString() << std::endl;
    }

}





