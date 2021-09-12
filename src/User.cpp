
#include "../include/User.h"
#include "../include/Tags.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Tags.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <bits/stdc++.h>


//Constructor
User::User(const std::string name) : name{name} ,copyConst{false}, history{} {
};
User::~User(){
    if(this -> copyConst){
        for(Watchable* h : this -> history){
            delete h;
            h = nullptr;
        }
    }
};

//////
//copy constructor
User::User (const User &other): name{other.name} ,copyConst{true}, history{}{
    for(const Watchable* con : other.history){
        this -> history.push_back(con->clone() );
    }
}

//move constructor
User::User ( User &&other) : name{other.name} ,copyConst{other.copyConst}, history{}
{
    for(const Watchable* con : other.history){
        this -> history.push_back(con->clone() );
    }

    other.history.clear();
}


//copy assignment operator
User& User::operator=(const User &other) {

    if(&other != this) {

        for (Watchable *wa : this->history) {
            delete wa;
            wa = nullptr;
        }
        this->history.clear();

        this->name = other.name;
    }
    return *this;
}

//assignment operator
User& User::operator=(User &&other) {

    if(&other != this) {

        for (Watchable *wa : this->history) {
            delete wa;
            wa = nullptr;
        }
        this->history.clear();

        this->name = other.name;
    }
    return  *this;
}
/////////
//Methods
std::string User::getName() const{
    return this -> name;
}

std::vector<Watchable *> User::get_history() {
    return history;
}

void User::addContent(Watchable *w, Session& s) {
    this->get_history().push_back(w);
}



void User::set_history(std::vector<Watchable *> history0) {
    int i = 0;
    for( Watchable* wa : history0){
        history.at(i) = wa;
        i++;
    }
}

void User::addToHistory(Watchable *w) {
    history.push_back(w);

}


//LengthRecommenderUser
//Constructor
LengthRecommenderUser::LengthRecommenderUser(std::string name)
        : User(name) {};



//Methods
int LengthRecommenderUser::calcAvg (){
    int sum= 0 ; int num = history.size() ;
    for(Watchable* i : history)
    {
        //the length of the item in the history vector
        sum+= i->getLength();

    }
    return (sum / num) ;
}


Watchable* LengthRecommenderUser::getRecommendation(Session& s){
//calculate the user's watch history content's lengths average
//find the content ellement with the closest value ot the avrage value
    int minDiff = s.getActiveUser()->get_history().at(0)->getLength();

    int avg = 0 ;
    avg = calcAvg();
    std::vector<Watchable*> availableContent = this->Select(s.getContent() , s.getActiveUser()->get_history());
//now we need to find the item to recommend from the vec vector
    Watchable* picked = nullptr ;
    int i = 0;

    for(Watchable* j : availableContent){
        if (abs(j->getLength()-avg) < minDiff){
            minDiff = abs(j->getLength()-avg);
            picked = j; //Copy the pointer
        }
        i++;
    }

    return picked;
}

User* LengthRecommenderUser::clone() const{
    return new LengthRecommenderUser(*this);
}



std::string LengthRecommenderUser::getType() {
    return "len";
}

std::vector<Watchable *> LengthRecommenderUser::Select(std::vector<Watchable *> c, std::vector<Watchable *> h) {
    std::vector<Watchable*> vec1;
    bool canAdd;
    for(Watchable* i : c) {
        canAdd = true;
        while (canAdd) {
            for (Watchable *j : h) {
                if (i->getID() == j->getID())
                    canAdd = false;
            }
            if (canAdd) {
                vec1.push_back(i);
                canAdd = false;
            }
        }
    }
    return vec1;
}

void LengthRecommenderUser::addContent(Watchable *w, Session &s) {

}


User *RerunRecommenderUser::clone() const{
    return new RerunRecommenderUser(*this);
}

//2
//Constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string name) :
        User(name)  {};

//Methods
std::string RerunRecommenderUser::getType() {
    return "rer";
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s){

    int n = s.getActiveUser()->get_history().size();
    Watchable* picked = s.getActiveUser()->get_history().at(n-1);
    return picked;
}

void RerunRecommenderUser::addContent(Watchable *w, Session &s) {

}

std::vector<Watchable *> RerunRecommenderUser::Select(std::vector<Watchable *> c, std::vector<Watchable *> h) {
    return c;
}


//3
//Constructor
GenreRecommenderUser::GenreRecommenderUser(const std::string name) : User(name), tagList{} {}
//Methods
User *GenreRecommenderUser::clone() const{
    return new GenreRecommenderUser(*this);
}

std::string GenreRecommenderUser::getType() {
    return "gen";
}

void GenreRecommenderUser::addContent(Watchable *w, Session& s) {
    this->get_history().insert(this->get_history().end(), w);
    int i = 0;
    for (std::string tag: w->getTags()) {
        bool isExist = false;
        for (Tags* t: this->getTagList()) {
            if (!isExist) {
                if (tag == t->getTagName()) {
                    t->increaseCount();
                    isExist = true;
                    int j = 0;
                    while (j < i) {
                        for (Tags* r: this->getTagList()) {
                            if (r->getCountTag() < t->getCountTag()) {
                                std::swap(t, r);
                            }
                        }
                        j++;
                    }

                }
                i++;
            }
            if (!isExist) {
                Tags *t1 = new Tags(tag);
                //int n = getTagList().size();
                getTagList().insert(getTagList().end(), t1);
            }
        }
    }

}






std::vector<Watchable*> GenreRecommenderUser::Select(std::vector<Watchable*> c, std::vector<Watchable*> h) {
    std::vector<Watchable*> vec1;
    bool canAdd;
    for(Watchable* i : c) {
        canAdd = true;
        while (canAdd) {
            for (Watchable *j : h) {
                if (i->getID() == j->getID())
                    canAdd = false;
            }
            if (canAdd) {
                vec1.push_back(i);
                canAdd = false;
            }
        }
        }
        return vec1;
    }




std::vector<Tags*> GenreRecommenderUser::getTagList() {
    return tagList;
}
Watchable *GenreRecommenderUser::getRecommendation(Session &s){
    //Tags *tag;
    bool found = false;
    std::vector <Watchable*> availableContent = Select(s.getContent(),s.getActiveUser()->get_history());
    for(Tags *t: this->getTagList()) {
        if (!found) {
            for (Watchable *w: availableContent) {
                for (std::string s: w->getTags()) {
                    if (s == t->getTagName()) {
                        found = true;
                        return w;
                    }
                }
            }
        }
    }
    return nullptr;
}