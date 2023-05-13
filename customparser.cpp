// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";


void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    // TO DO - Add your code below.
    int numContents;
    is >> numContents;
    
    if(is.fail()) {
        throw ParserError(error_msg_1);
    }
    //cout << contID << " " << contType;
    for(int i = 0; i < numContents; i++) {
        int contID; 
        int contType;
        is >> contID;
        is >> contType;
        //cout << contID << " " << contType;
        std::string finalWord;
        //cout << contID << " " << contType;
        
        //get whitespace after type
        char c;
        is.get(c);
        
        std::getline(is >> std::ws, finalWord);
        /*std::stringstream ss(contentName);
        std::string word;
        std::string finalWord;
        while (ss >> word) {
          finalWord += word;
        }*/
        //cout << finalWord << endl;
        int numRevs; 
        is >> numRevs;
        //cout << numRevs << endl;

        int totStars;
        is >> totStars;
        //cout << totStars << endl;

        int contRating;
        is >> contRating;
        //cout << contRating << endl;

        int numEps;
        if(contType == 1) {
          is >> numEps;
        }
        //cout << numEps << endl;
        
        Content* newC = NULL;
        if(contType == 0) {
          newC = new Movie(contID, finalWord, numRevs, totStars, contRating);
        }
        else {
          newC = new Series(contID, finalWord, numRevs, totStars, contRating, numEps);
        }

        //std::string curr_line;
        //while(getline(is, curr_line)){ cout << curr_line << endl; }

        //get whitespace after numeps 
        std::string garbage_string;
				getline(is, garbage_string);
        std::string garbage;
        
        
        std::string userName;
        //getline(is,userName);
        //cout << userName;
        bool out = false;
        int count = 0;
          while(std::getline(is, userName) && !userName.empty() ) {
          //std::getline(is, userName);
            //cout << userName << "end" << endl;
            std::stringstream ss2(userName);
            std::string user1;
            while (ss2 >> user1) {
              newC->addViewer(user1);
              //count = count + 1;
              
              //cout << user1 << " here " << endl;
            }
          }

        

        
        content.push_back(newC);
        /*counter = counter + 1;
        if(counter == 3) {
          char ch;
          is.get(ch);
          std::string garbageStr;
          getline(is, garbageStr);
        }*/
        
        if(is.fail()) {
          throw ParserError(error_msg_2);
        }
    }
  int uCounter = 0;
  std::string blankLine;
  while(std::getline(is >> std::ws, blankLine) && !blankLine.empty()) {
    //std::cout << blankLine;
    std::stringstream ss3(blankLine);
    std::string usernam;
    ss3 >> usernam;
    //cout << usernam << "username" << endl;
    
    int ratingLim;
    ss3 >> ratingLim;
    //cout << ratingLim << "ratingLIm" << endl;
    

    if(!isdigit(ratingLim)) {
      throw ParserError(error_msg_3);
    }

    User* us = new User(usernam, ratingLim);
    
    
    //else {
    int viewID;
    std::string viewIDline;
    std::getline(is, viewIDline);
    cout << viewIDline << " end";
    std::stringstream ss4(viewIDline);
    while(ss4 >> viewID) {
      //cout << viewID<< "end";
      us->addToHistory(viewID);
    }
    //}
    
    users.push_back(us);
    
    if(is.fail()) {
      throw ParserError(error_msg_3);
    }
  }
}
