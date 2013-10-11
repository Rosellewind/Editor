//
//  Display.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/10/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#include "Display.h"
#include <sstream>


Display::Display(BufferManager *bufferManager){
    searchWord = "";
    isSearch = displayResult = resultTrue = false;
    buffer = bufferManager;
    searchPoint = 0;
}

void Display::startSearch(int point){
    isSearch = true;
    searchPoint = point;
}

void Display::stopSearch(){
    isSearch = false;
    searchWord = "";
}

string Display::intToString(int i){//////////////
    string s;
    stringstream out;
    out << i;
    s = out.str();
    return s;
}

int Display::stringSearch(const char *pattern, const char *text){
	int i, j, lengthOfPattern= (int)strlen(pattern),lengthOfText= (int)strlen(text);
    
    //increments i and j on matches, j is reset and i adjusted if no match
	for (i = searchPoint, j = 0; j<strlen(pattern) && i<lengthOfText; i++,j++){
        if (pattern[j] == '*') {
            while (text[i+1] == pattern[j+1]) {i++; lengthOfPattern++;}
            i--; lengthOfPattern--; continue;
        }
        
        while(i<lengthOfText && pattern[j] != '?' && pattern[j] != '*' && text[i]!=pattern[j]){
            if ((strlen(pattern) > 0 && pattern[j-1] == '*')) {
                j--; lengthOfPattern++;}
            else {i=i-j+1; j = 0;}
        }
	}
	if(j==strlen(pattern)) return i-lengthOfPattern; else return -1;
}

void Display::insert(string str){
    for (int i = 0; i < str.length(); i++){
        if (str[i] == 10){                             //enter
            int index = stringSearch(searchWord.c_str(), buffer->getBuffer());
            if (index == -1)
                index = searchPoint;
            else resultTrue = true;
            buffer->setPointA(index);
            stopSearch();
            displayResult = true;
        }
        else
            searchWord += str[i];
    }
}

void Display::redisplay(BufferManager *buffer){
    
    //display text
    clear();
    string left = buffer->leftString();
    string right = buffer->rightString();
    addstr(left.c_str());
    addstr(right.c_str());
    
    position pos;
    if(isSearch){
        pos.y = LINES - 1;
        pos.x = 12;
    }
    else pos = buffer->getPosition(COLS, LINES);
    
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
        string statusString = "Search for: ";
        addstr(statusString.c_str());
        addstr(searchWord.c_str());
        move(LINES - 1,(int)statusString.length() + (int)searchWord.length());
        refresh();
    }
    
    //display status
    else{
        move(LINES-1, 0);
        string statusString = "Curser: " + intToString(pos.y) + "," + intToString(pos.x) + "  ";
        addstr(statusString.c_str());
        addstr(buffer->varString().c_str());
        move(pos.y,pos.x);
        refresh();
    }
}
