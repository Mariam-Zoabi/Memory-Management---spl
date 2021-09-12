#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>


class Watchable;
class Session;
class Tags;

class User{
    //Fields
private:
    std::string name;
    bool copyConst;

protected:
    std::vector<Watchable*> history;
public:
    //Constructors
    User(const std::string name);
    User();
    /////
    User (const User &other);//copy constructor
    User ( User &&other);//move constructor

    User& operator = (const User &other);//copy assignment operator
    User& operator = (User &&other); //assignment operator
    /////
    virtual User* clone() const =0;//+
    virtual ~User();
    //Methods
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual  std::string getType() = 0 ;//+
    std::string getName() const;//+
    std::vector<Watchable*> get_history();
    void set_history(std::vector<Watchable*>);
    virtual void addContent(Watchable *w, Session& s) = 0;
    void addToHistory(Watchable *w);

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(std::string name);
    virtual Watchable* getRecommendation(Session& s);
    int calcAvg();
    std::string getType();//+
    User* clone() const ;//+
    std:: vector<Watchable*> Select (std::vector<Watchable*> c, std::vector<Watchable*> h);
    virtual void addContent(Watchable *w, Session& s);
private:

};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string name);
    virtual Watchable* getRecommendation(Session& s);
    std::string getType();//+
    User* clone() const;//+
    std:: vector<Watchable*> Select(std::vector<Watchable *> c, std::vector<Watchable *> h);
    virtual void addContent(Watchable *w, Session& s);
private:
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string name);
    std::vector<Tags*> getTagList();
    virtual Watchable* getRecommendation(Session& s);
    std::string getType();//+
    std::vector<Watchable*> Select(std::vector<Watchable*> c, std::vector<Watchable*> h);
    User* clone() const;//+
    void addContent(Watchable *w, Session& s);
private:
    std::vector<Tags*> tagList;


};

#endif