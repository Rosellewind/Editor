//
//  Display.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/10/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#include "Display.h"
#include <sstream>


Display::Display(BufferManager *bufferManager){             //initialize class instance
    searchWord = "";
    isSearch = displayResult = resultTrue = false;
    buffer = bufferManager;
    searchPoint = 0;
}

void Display::startSearch(int point){                       //called to start a search
    isSearch = true;
    searchPoint = point;
}

void Display::stopSearch(){                                 //called to stop a search
    isSearch = false;
    searchWord = "";
}

string Display::intToString(int i){                         //makes string out of integer
    string s;
    stringstream out;
    out << i;
    s = out.str();
    return s;
}

void Display::insert(string str){                           //insert into search string
    for (int i = 0; i < str.length(); i++){
        if (str[i] == 10){                                  //if enter key, proceed with search
            int index = buffer->searchF(searchWord.c_str(), searchPoint);
            if (index == -1){                               //if search failed,
                index = searchPoint;
            //    buffer->setPointA(index, false);
            }
            else{                                           //
                resultTrue = true;
                buffer->setPointA(index, false);
            }
            stopSearch();
            displayResult = true;
        }
        else
            searchWord += str[i];
    }
}

void Display::redisplay(BufferManager *buffer){             //update the display
    
    //display text
    clear();
    string bufferString = buffer->bufferString();
    addstr(bufferString.c_str());
    
    //position cursor
    position pos;
    if(isSearch){
        pos.y = LINES - 1;
        pos.x = 12;
    }
    else pos = buffer->getCurserPosition(COLS, LINES);
    
    //display search result
    if (displayResult){
        move(LINES-1, 0);
        string statusString = resultTrue ? "true" : "false";
        addstr(statusString.c_str());
        move(pos.y,pos.x);
        displayResult = resultTrue = false;
        refresh();
    }
    
    //display search
    else if (isSearch){
        move(LINES-1, 0);
        string statusString = "  Search for: ";
        addstr(statusString.c_str());
        addstr(searchWord.c_str());
        move(LINES - 1,(int)statusString.length() + (int)searchWord.length());
        refresh();
    }
    
    //display status
    else{
        move(LINES-1, 0);
        string statusString = "";
        if (buffer->getOverstrike()) statusString = "OVERSTRIKE  ";
        statusString += "Curser: " + intToString(pos.y) + "," + intToString(pos.x) + "  ";
        addstr(statusString.c_str());
        addstr(buffer->varString().c_str());
        move(pos.y,pos.x);
        refresh();
    }
}
