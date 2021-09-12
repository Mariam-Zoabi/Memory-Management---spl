#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include <unordered_map>
#include <string>
#include <iostream>
#include "User.h"


class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
private:
    std::string command;//+
    std::string errorMsg;
    ActionStatus status;
public:
    virtual ~BaseAction()=0;//+
    void setCommand(std::string);//+
    BaseAction();
    //methods
    ActionStatus getStatus() const;
    void setStatus(ActionStatus status);
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() const =0;//+
protected:
    std::string getCommand() const;//+
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;

};



class CreateUser  : public BaseAction {
public:
    CreateUser(std::string name, std::string type); //+
    CreateUser(const CreateUser &other);//+
    CreateUser(CreateUser &&other);//+
    ~CreateUser();//+
    virtual CreateUser* clone() const;//+
    virtual void act(Session &sess);
    virtual std::string toString() const;

private:
    std::string name;//+
    std::string type;//+

};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(std::string name);//+
    ChangeActiveUser(const ChangeActiveUser &other);//+
    ChangeActiveUser(ChangeActiveUser &&other);//+
    ~ChangeActiveUser();//+
    virtual ChangeActiveUser* clone() const;//+
    virtual void act(Session& sess);

    virtual std::string toString() const;

private:
    std::string userName;

};

class DeleteUser : public BaseAction {
public:
    DeleteUser(std::string& name);//+
    DeleteUser(const DeleteUser &other);//+
    DeleteUser(DeleteUser &&other);//+
    ~DeleteUser();//+
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual DeleteUser* clone() const;//+
private:
    std::string name;//+

};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(std::string name1, std::string name2);//+
    DuplicateUser(const DuplicateUser &other);//+
    DuplicateUser(DuplicateUser &&other);//+
    ~DuplicateUser();//+
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual DuplicateUser* clone() const;//+
private:
    std::string name1;
    std::string name2;
};

class PrintContentList : public BaseAction {//5
public:

    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual PrintContentList* clone() const;//+
};


class PrintWatchHistory : public BaseAction {//6
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual PrintWatchHistory* clone() const;//+
};


class Watch : public BaseAction {//7
public:
    Watch(long id);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual Watch* clone() const;//+
private:
    long id;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual void act(Session &sess);
    std::string toString() const;
    virtual PrintActionsLog* clone() const;
    ~PrintActionsLog();
private:
};

class Exit : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual Exit* clone() const;//+
};
#endif