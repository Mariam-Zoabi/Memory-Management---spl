//
// Created by mariam on 12/3/19.
//

#include "../include/Tags.h"
#include <string>
#include <vector>


Tags::Tags(std::string tagName)
        : tagName{tagName}, tagCount{1}{}

std:: string Tags::getTagName() const{
    return this->tagName;
}

int Tags:: getCountTag() {
    return this->tagCount;
}
void Tags:: increaseCount() {
    this->tagCount++;
}