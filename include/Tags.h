//
// Created by mariam on 12/3/19.
//
#include <string>
#ifndef SPLFLIX_TAGS_H
#define SPLFLIX_TAGS_H


class Tags {
private:
    std:: string tagName;
    int tagCount;
public:
    Tags(std::string tagName);

    std:: string getTagName() const;
    int getCountTag();
    void increaseCount();
};


#endif //SPLFLIX_TAGS_H