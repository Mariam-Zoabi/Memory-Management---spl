
#include "../include/User.h"
#include "../include/Tags.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Tags.h"

#include <string>
#include <vector>



//Constructor
Watchable::Watchable(long id, int length, const std::vector<std::string>& tags)
        :id{id} , length{length} , tags{tags}{}
//Destructor
Watchable::~Watchable(){}

int Watchable::getLength() const {
    return length;
}

long Watchable::getID() const {
    return id;
}

std::vector<std::string> Watchable::getTags() const {
    return tags;
}


//Constructor
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags)
        :Watchable( id,  length,  tags) , name{name}{}


//Methods
bool Movie::is_movie() {
    return true;
}

Watchable *Movie::clone() const {
    return new Movie(*this);
}

std::string Movie::toString() const {
    std::string idString=std::to_string(getID());
    std:: string lengthString = std:: to_string(getLength());
    std:: string tagsString=",";
    //int i=0;
    for(std::string s: getTags())
    {
        tagsString = tagsString +", " + s;
    }

    return idString + ". " + name + " " + lengthString + " minutes ["+ tagsString.substr(3,tagsString.size()-1) +"]";
}

std:: string Movie:: getName() const{
    return this->name;

}

Watchable* Movie:: getNextWatchable(Session& ses) const{
    return nullptr;
}

Movie::~Movie() {};
//2.
Watchable* Episode:: getNextWatchable(Session& ses) const{
    long idNum = ses.getActiveUser()->get_history().at(ses.getActiveUser()->get_history().size()-1)->getID()-1;
    long nextIDNum = idNum +1;
    if(ses.getContent().at((int) idNum)->getName() == ses.getContent().at((int) nextIDNum)->getName())
        return ses.getContent().at((int) nextIDNum);
    else
        return nullptr;
}



//Constructor
Episode::Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags)
        :Watchable( id,  length,  tags) , seriesName{seriesName} ,
         season{season}, episode{episode}, nextEpisodeId{nextEpisodeId}{}

std::string Episode::toString() const {
    std::string idString=std::to_string(getID());
    std:: string lengthString = std:: to_string(getLength());
    std:: string tagsString = ", ";
    //int i=0;
    for(std::string s: getTags())
    {
        tagsString = tagsString+ ", " + s;
    }

    std:: string seasonString = std:: to_string(season);
    std:: string episodeString = std:: to_string(episode);

    return idString + ". " + seriesName + " S" +seasonString + "E" +episodeString+ " " +  lengthString + " minutes ["+ tagsString.substr(4,tagsString.size()-1) +"]";
}

bool Episode::is_movie() {
    return false;
}

Watchable *Episode::clone() const{
    return new Episode(*this);
}

std:: string Episode:: getName() const{
    return this->seriesName;

}
//Destructor
Episode::~Episode(){}