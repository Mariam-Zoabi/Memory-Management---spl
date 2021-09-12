#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
public:
    //Constructors
    Session(const std::string &configFilePath);//, std:: vector<Watchable*> content, std::vector<BaseAction*> actionsLog, std:: unordered_map<std::string, User*> userMap, User* activeUser);
    //copy constructor
    Session(const Session &other);
    //move constructor
    Session(Session &&other);

    //destructor
    ~Session();

    //methods
    //copy assignment operator
    Session& operator = (const Session &other);
    //assignment operator
    Session& operator = (Session &&other);

    void start();
    bool isValidType(std::string type) const;
    std::vector<Watchable*> getContent() const; //+
    std::vector<BaseAction*> getActionsLog();//+
    std::unordered_map<std::string,User*> getUserMap();//+
    void SetActiveUser(User* user);//+
    User* getActiveUser();//+
    void addUserToMap(std:: string key, User *newUser);
    void deleteFromMap (std:: string key);
    void addWatchableToUser(Watchable* w);
    void showLog();

};
#endif