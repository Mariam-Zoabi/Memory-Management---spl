

#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "../include/User.h"
#include "../include/Tags.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Tags.h"


//Constructors
BaseAction::BaseAction()
        : command{}, errorMsg{} , status{PENDING} {}

BaseAction::~BaseAction(){};

//Methods
ActionStatus BaseAction::getStatus() const {
    return this -> status;
}

void BaseAction::complete(){
    this -> status = COMPLETED;
}

void BaseAction::error(const std::string& errorMsg){
    this -> status = ERROR;
    this->errorMsg = errorMsg;
    std::cout<<getErrorMsg() << std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return this->errorMsg;
}

void BaseAction::setCommand(std::string cmd) {
    this->command = cmd;
}

void BaseAction::setStatus(ActionStatus status1) {

    this->status = status1;
}

std::string BaseAction::getCommand() const  {
    return this->command;
}


//2.
//Constructor

//Copy Constructor
CreateUser::CreateUser(const CreateUser &other)
        : BaseAction(other) , name{other.name}, type{other.type} {}

//Move Constructor
CreateUser::CreateUser(CreateUser &&other)
        : BaseAction(other) , name{other.name} , type{other.type}{}

//Destructor
CreateUser::~CreateUser(){/*
    if(this -> getStatus() != COMPLETED){
            for(std::unordered_map<std::string, User*>::iterator itr = this -> userMap.begin(); itr != this -> userMap.end(); itr++)//todo !!
            {
                delete (itr->second);
            }
        this -> userMap.clear();
        }*/
}


CreateUser* CreateUser::clone() const {
    return  (new CreateUser(*this));
};

CreateUser::CreateUser(std::string name , std::string type)
        : BaseAction() , name{name} ,type{type} {
}


//Methods
void CreateUser::act(Session &sess) {
    if(!(sess.isValidType(type))){
        error("Error User's Type");
        //return;
    }

    else{
        bool notExt = true;
        for(std::pair<std::string, User*> element : sess.getUserMap())
        {

            if(element.second->getName() == this->name)
                notExt = false;
        }

        if(!notExt)
            error("There is Already a User With The Same Name");

        else {

            if (type == "len") {
                std::string tempName = this->name;
                User* newUser = new LengthRecommenderUser(tempName);
                sess.addUserToMap(tempName, newUser);
            }
            if (type == "rer") {
                std::string tempName = this->name;
                User* newUser = new RerunRecommenderUser(tempName);
                sess.addUserToMap(tempName, newUser);
            }
            if (type == "gen") {
                std::string tempName = this->name;
                User *newUser = new GenreRecommenderUser(tempName);
                sess.addUserToMap(tempName, newUser);
            }


            complete();
        }

    }
};



std::string CreateUser::toString() const {
    std::stringstream ss;
    ss << this->getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
};

//for (std::pair<std::string, User*> element : userMap)

//3.
//Constructor
ChangeActiveUser::ChangeActiveUser(std::string name) : BaseAction() , userName{name} {}

ChangeActiveUser::~ChangeActiveUser(){}
//Copy Constructor
ChangeActiveUser::ChangeActiveUser(const ChangeActiveUser &other): BaseAction() , userName{other.userName} {};//todo

void ChangeActiveUser::act(Session &sess) {
    bool notExt = false;
    std::unordered_map<std::string,User*>::const_iterator got = sess.getUserMap().find (userName);
    if(got == sess.getUserMap().end())
        notExt = true;
    if(notExt){
        error("This user name does not exist");
    }
    else{
        User* u = sess.getUserMap().at(userName);
        sess.SetActiveUser(u);
    }

}

ChangeActiveUser* ChangeActiveUser::clone() const {
    return  (new ChangeActiveUser(*this));
};


std::string ChangeActiveUser::toString() const {
    std::stringstream ss;
    ss << this->getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
};

//4.
// todo: copy/move/destructor missing
DeleteUser::DeleteUser(std::string& name) : BaseAction() , name{name}{}

void DeleteUser::act(Session &sess) {
    bool notExt = false;
    std::string tempName = this->name;
    std::unordered_map<std::string,User*>::const_iterator got = sess.getUserMap().find (tempName);
    if(got == sess.getUserMap().end())
        notExt = true;
    if(notExt){
        error("This user name does not exist");
    }
    else{
        sess.deleteFromMap(tempName);
    }
}

std::string DeleteUser::toString() const {
    std::stringstream ss;
    ss << this->getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
};


DeleteUser* DeleteUser::clone() const {
    return  (new DeleteUser(*this));
}

DeleteUser::DeleteUser(const DeleteUser &other) : BaseAction() , name{other.name}{}

DeleteUser::~DeleteUser() {}

DeleteUser::DeleteUser(DeleteUser &&other) :BaseAction() , name{other.name} {};

//5.
DuplicateUser::DuplicateUser(std::string name1, std::string name2) : BaseAction() , name1{name1} , name2{name2}{};


void DuplicateUser::act(Session &sess) {
//get tow names, if the first is not there or the second already there -> error
    bool ext1 = true;
    bool ext2;
    std::unordered_map<std::string, User *>::const_iterator got1 = sess.getUserMap().find(name1);
    if (got1 == sess.getUserMap().end())
        ext1 = false;
    std::unordered_map<std::string, User *>::const_iterator got2 = sess.getUserMap().find(name2);
    if (got2 == sess.getUserMap().end())
        ext2 = false;
    else{
        ext2 = true;
    }
    if (ext1 & !ext2) {
        User *u1 = sess.getUserMap().at(name1);
        std::string s = u1->getType();
        User *u2;
        if (s == "len")
            u2 = new LengthRecommenderUser(name2);
        if (s == "rer")
            u2 = new RerunRecommenderUser(name2);
        if (s == "gen")
            u2 = new GenreRecommenderUser(name2);

        u2->set_history(u1->get_history());
        sess.addUserToMap(name2,u2);

    }
    else error("The old username does not exist or the New Username is already taken");
}

std::string DuplicateUser::toString() const {
    std::stringstream ss;
    ss << this->getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";
    return ss.str();
};


DuplicateUser* DuplicateUser::clone() const {
    return  (new DuplicateUser(*this));
}

DuplicateUser::~DuplicateUser() {}

DuplicateUser::DuplicateUser(DuplicateUser &&other) : name1{other.name1},name2{other.name2} {}

DuplicateUser::DuplicateUser(const DuplicateUser &other): name1{other.name1},name2{other.name2} {
    //this -> name1 = other.name1;
    //this -> name2 = other.name2;
}
//6.

void PrintContentList::act(Session &sess) {
    for(Watchable* a : sess.getContent()){
        std::cout <<a->toString() << std::endl;
    }
}

std::string PrintContentList::toString() const {
    std::stringstream ss;
    ss << this-> getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
}

PrintContentList *PrintContentList::clone() const {
    return  (new PrintContentList(*this));
}

//7.
Watch::Watch(long id) : BaseAction() , id{id} {}


void Watch::act(Session &sess) {


    Watchable *con = sess.getContent().at(id-1);
    std::cout << "Watching" << con->toString().substr(2, con->toString().size()-1) << std::endl;
    User* curr = sess.getActiveUser();
    curr->addToHistory(con);
    curr->addContent(con, sess);
    sess.addWatchableToUser(con);
    if(con->getNextWatchable(sess) != nullptr) {
        std::cout << "We recommend watching " << con->getNextWatchable(sess)->toString().substr(4,con->getNextWatchable(sess)->toString().size() -1)<< ", continue watching? [y/n]" << std::endl;
        std::string input;
        getline(std::cin, input);
        if (input == "y") {
            BaseAction *b = new Watch(con->getNextWatchable(sess)->getID());
           b->act(sess);
        }
    }
    else {
        std::cout << "We recommend watching " << curr->getRecommendation(sess)->getName() << ", continue watching? [y/n]" << std::endl;
        std::string input;
        getline(std::cin, input);
        if (input == "y") {
            BaseAction *b = new Watch(curr->getRecommendation(sess)->getID());
           b->act(sess);
        }
    }
   complete();


     }

std::string Watch::toString() const {
    std::stringstream ss;
    ss << this-> getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
}

Watch *Watch::clone() const {
    return (new Watch(*this));
}


void PrintWatchHistory:: act (Session& sess) {
    int i = 1;

    std::string dot = ".";

    for (Watchable *a : sess.getActiveUser()->get_history()) {
        size_t find = a->toString().find(dot);
        std::string s = std::to_string(i);
        std::cout << s << a->toString().substr(find, a->toString().length()) << std::endl;
        i++;
    }
}

std::string PrintWatchHistory::toString() const {
    std::stringstream ss;
    ss << this-> getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
}

PrintWatchHistory *PrintWatchHistory::clone() const {
    return (new PrintWatchHistory(*this));
}
//8.

PrintActionsLog::PrintActionsLog(): BaseAction() {}

PrintActionsLog* PrintActionsLog::clone() const {
    return (new PrintActionsLog(*this));
}

PrintActionsLog::~PrintActionsLog() {}

void PrintActionsLog::act(Session &sess) {

    sess.showLog();
    complete();
}

std::string PrintActionsLog::toString() const {
    std::stringstream ss;
    ss << this->getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
}

//
void Exit::act(Session &sess) {
    this -> complete();
}

std::string Exit::toString() const {
    std::stringstream ss;
    ss << this->getCommand();
    if(getStatus() == ERROR)
        ss << " Error: " << this->getErrorMsg();
    else // if(getStatus() == COMPLETED)
        ss << " Completed";

    return ss.str();
}

Exit *Exit::clone() const {
    return (new Exit(*this));
}