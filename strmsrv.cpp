#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;


StreamService::StreamService()

{
    cUser_ = NULL;
}

StreamService::~StreamService()
{
    //delete <pointer> content and user vectors 
    for(int i = 0; i < content_.size(); i++) {
        delete content_[i];
    }
    content_.clear();

    for(int i = 0; i < users_.size(); i++) {
        delete users_[i];
    }
    users_.clear();

}

void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

void StreamService::userLogin(const std::string& uname)
{
    if(cUser_) { // checks for already logged in user
        throw std::runtime_error("runtime_error");
    }
    
    bool userExists = false; // check if user exists 
    for(size_t i = 0; i < users_.size(); i++) {
        if(users_[i]->uname == uname) {
            userExists = true;
            cUser_ = users_[i];
        }
    } 
    
    //if user does not exist, throw
    if(userExists == false) {
        throw std::invalid_argument("invalid_argument");
    }
}

void StreamService::userLogout()
{
    if(!cUser_) {
        return;
    }
    cUser_ = nullptr;
}


std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        if(content_[i]->name().find(partial) != std::string::npos) { 
            results.push_back(i);
        }
        if(partial == "*" /* update me */){
            results.push_back(i);
        }        
    }
    return results;
}

std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

void StreamService::watch(CID_T contentID)
{
    throwIfNoCurrentUser(); // checks if user is logged in

    if(isValidContentID(contentID) == 0) {
        throw ReviewRangeError("range_error");
    }

    //checks if rating is above User's rating limit 
   bool goodRating = false;
   int contentRating = content_[contentID]->rating();
   if(contentRating <= cUser_->ratingLimit) {
        goodRating = true;
    }
    if(goodRating == false) {
        throw RatingLimitError("rating_limit_error");
    }
    //if it is a good rating, and valid contentID, then add to viewer history
    //and add viewer watch to list;
    if(isValidContentID(contentID) && goodRating == true) {
        cUser_->history.push_back(contentID);
        content_[contentID]->addViewer(cUser_->uname);
    }
}

void StreamService::reviewShow(CID_T contentID, int numStars)
{
    throwIfNoCurrentUser();
    if(isValidContentID(contentID) == false) {
        throw ReviewRangeError("range_error");
    }

    //if stars is between 0-5
    if(numStars<0 || numStars > 5) {
        throw ReviewRangeError("range_error");
    }
    //if its is a valid contentid, then add review of given stars
    if(isValidContentID(contentID)) {
        content_[contentID]->review(numStars);
    }

}

CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    // Change this when you are ready, but for now it is a 
    // dummy implementation
    throwIfNoCurrentUser();
    if(!isValidContentID(contentID)) {
        throw range_error("range_error");
    }

    std::vector<std::string> viewers = content_[contentID]->getViewers();
    std::vector<std::string> viewers2; 
    std::vector<CID_T> allIDs;
    std::vector<CID_T> histIDs;

    //captures all users who viewed the contentID except cUser and stores in vector
    for(int i = 0; i < viewers.size(); i++) {
        if(viewers[i] != cUser_->uname) {
            viewers2.push_back(viewers[i]);
        }
    }
    
    //captures all IDS of other content the other users have watched
    for(int j = 0; j < viewers2.size(); j++) {
        for(int k = 0; k < users_.size(); k++) {
            if(viewers2[j] == users_[k]->uname) {
              //histIDs = users_[k]->history;
              for(int m = 0; m < users_[k]->history.size(); m++) {
                    allIDs.push_back(users_[k]->history[m]);
                }
            }
        } 
    }
    //take out the current contentID
    for(int id = 0; id < allIDs.size(); id++) {
      if(allIDs[id] == contentID) {
        allIDs.erase(allIDs.begin() + id);
      }
    }

    /*for(int ids = 0; ids < allIDs.size(); ids++) {
      cout << allIDs[ids] << " ";
     }*/

    //bool watch = false;
    //std::vector<CID_T> unwatchedIds; 


    //take out the content that the current User has already watched
    //std::vector<CID_T> cUserHist = cUser_->history;
    for(int q = 0; q < allIDs.size(); q++) {
        for(int w = 0; w < cUser_->history.size(); w++) {
            if(cUser_->history[w] == allIDs[q]) {
                allIDs.erase(allIDs.begin() + q);
            }
        }
    }


    for(int ids = 0; ids < allIDs.size(); ids++) {
      cout << allIDs[ids]<< "here1";
    }
    

    if(allIDs.size() < 4 && allIDs[0] == 0) {
      cout << "here";
    }
    

    //only keep content that was viewed by all other users
    else {
    for(int id1 = 0; id1 < allIDs.size(); id1++) {
      int counter = 0;
      for(int id2 = 0; id2 < allIDs.size(); id2++) {
        if(allIDs[id2] == allIDs[id1]) {
          counter++;
        }
      }
      if(counter < viewers2.size() ) {
        allIDs.erase(allIDs.begin() + id1);
      }
    }
    }
   
    for(int id = 0; id < allIDs.size(); id++) {
      cout << allIDs[id] << "here";
    }
    //2223

    //if no other content, return -1
    if (allIDs.size() == 0) {
      return -1;
    }

    //to find max views of content and get contentID of that content
    int max = 0;
    int maxID;
    for(int r = 0; r < allIDs.size(); r++) {
        for(int u = 0; u < content_.size(); u++) {
            if(allIDs[r] == content_[u]->id()) {
                if(content_[u]->getViewers().size() > max) {
                    max = content_[u]->getViewers().size();
                    maxID = content_[u]->id();
                }
            }
        }
    }
    
    return maxID;
}

void StreamService::displayContentInfo(CID_T contentID) const
{
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }
    content_[contentID]->display(cout);  

}


bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
