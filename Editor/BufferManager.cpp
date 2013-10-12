//
//  BufferManager.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/8/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#include "BufferManager.h"
#include <sstream>
#include <math.h>

//private methods
char* BufferManager::createBuffer(){                        //create the buffer
    char *buffer = new char[MAX_DOCUMENT_LENGTH];
    return buffer;
}

string BufferManager::intToString(int i){                   //puts an int into a string
    string s;
    stringstream out;
    out << i;
    s = out.str();
    return s;
}

bool BufferManager::crossesTheGap(int count){               //check if moving the point will cross the gap
    int total = point + count;
    bool cross = false;
    if (point == gapL || point == gapL - 1) cross = (count > 0);
    else if (point == gapR) cross = (count < 0);
    else if((point < gapL && total >= gapR)||
            (point > gapR && total < gapL)) cross = true;
    return cross;
}

void BufferManager::addToTheGap(){                          //add to the gap from the end

    int count = GAP_BLOCK_SIZE;
    if (count + end > MAX_DOCUMENT_LENGTH)
        cout<<"reached max characters"<<endl;
    int numToMove = end - gapR;
    
    for (int i = 0; i < numToMove; i++) {
        buffer[end + count - i - 1] = buffer[end - i - 1];
    }
    end += count; gapR += count;
}

int BufferManager::gapSize(){                               //returns the size of the gap
    return gapR-gapL;
}

void BufferManager::shiftGap(int count, bool toTheLeft){    //shift the gap
    if (toTheLeft){
        if (gapL == gapR) gapR = gapL = point;
        else{
            for (int i = 0; i < count; i++) {
                gapR--; gapL--;
                buffer[gapR] = buffer[gapL];
            }
        }
    }
    else
        for (int i = 0; i < count; i++) {
            buffer[gapL] = buffer[gapR];
            gapR++; gapL++;
        }
    point = gapL;
}

void BufferManager::checkGap(){                             //check to see if the gap should be shifted
    if (point > end) point = end;
    if (point == end) usingEndMarker = true;                //sets the usingEndMarker if needed
    else usingEndMarker = false;
    if (!usingEndMarker && point != gapL && point != gapR) {
        if (gapL == gapR) gapR = gapL = point;
        else {
            bool toTheLeft = (point < gapL);                //determine if point is left or right of gap
            int count = toTheLeft ? gapL-point: point-gapR; //how many to shift
            shiftGap(count, toTheLeft);                     //shift it
        }
    }
}


//public methods
BufferManager::BufferManager(){                             //initialize buffer class instance
    point = gapL = gapR = end = overstrike = usingEndMarker = 0;
    buffer = createBuffer();
}

void BufferManager::setPointA(int location, bool adjust){                //set the point to absolute location
    point = location;//////////
    if (adjust) {
        bool pastTheGap = (point > gapL);                       //adjust if crosses the gap
        if (pastTheGap) point += gapSize();
    }
}

void BufferManager::setPointR(int count){                   //set the point relative to where it is now
    if (point + count >=0 && point + count <= end){
        int newPoint = point;
        if (crossesTheGap(count)) {
            bool movingLeft = (count < 0);
            if (movingLeft) newPoint -= gapSize();
            else  newPoint += gapSize();
        }
        newPoint += count;
        if (newPoint >= 0) point = newPoint;
    }
}

void BufferManager::insert(string str){                     //insert string into the buffer
    if (overstrike){
        for (int i = 0; i < str.length(); i++) {            //insert char and update
            if (crossesTheGap(1)) point += gapSize();
            buffer[point] = str[i];
            if (point == end) end++;
            point++;
        }
    }
    else {
        checkGap();                                         //check the gap, move if needed
        if (!usingEndMarker && gapSize() < str.length()) addToTheGap();
        for (int i = 0; i < str.length(); i++) {            //insert char and update
            buffer[point] = str[i];
            point++;
            if (usingEndMarker) {
                if (end == gapL)
                {gapL++; gapR++;}
                end++;
            }
            else gapL++;
        }
    }
}

void BufferManager::myDelete(int count){                    //delete count number of characters
//bool crosses = crossesTheGap(count);
    if (((count < 0 && point + count >= 0) ||
         ((count > 0 && point + count <= end) &&
          !(count > 0 && point == gapL && gapR == end)))
        && point <= end) {
        checkGap();                                         //check the gap
        if (usingEndMarker){
            if (point + count <= gapR){
                point = end = gapR = gapL = end + count - gapSize();
            }
            else point = end = end + count;
        }
        else if (count < 0)
            point = gapL = gapL + count;                    //delete by moving markers
        else
            point = gapR = gapR + count;
    }
}

int BufferManager::searchF(const char *pattern, int searchPoint){
    const char *text = bufferString().c_str();              //set the text
    bool acrossGap = false;
    if (searchPoint > gapL){ searchPoint -= gapSize(); acrossGap = true;}      //adjust searchPoint
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
    if (acrossGap || (searchPoint <= gapL && i > gapL))////////
        i += gapSize();
	if(j==strlen(pattern)) return i; else return -1;
}

string BufferManager::bufferString(){                         //returns string to the left of the gap
    int sizeLeft = gapL;
    string strL(buffer, sizeLeft);
    
    int sizeRight = end - gapR;
    string strR(&buffer[gapR], sizeRight);
    return strL + strR;
}

string BufferManager::varString(){                          //returns variable information string
    string str = "point:" + intToString(point) + "  gapL:" + intToString(gapL) +
    "  gapR:" + intToString(gapR) + "  end:" + intToString(end) + "  gapSize:" + intToString(gapSize()) + "  atPoint:" + intToString(buffer[point]);
    return str;
}

position BufferManager::getCurserPosition(int cols, int rows){    //cursor position for a window size
    position pos;
    pos.x = pos.y = 0;
    if (point == gapR) point = gapL;
    for (int i = 0; i < point; i++) {
        bool atTheGap = (i == gapL);       //adjust if crosses the gap
        if (atTheGap) i += gapSize();
        int c = buffer[i];
        if (c == 10){pos.x = 0; pos.y++;}               //return key
        else if (pos.x == cols) {pos.x = 1; pos.y++;}   //end of screen line
        else {pos.x++;}
    }
    return pos;
}

int BufferManager::getPoint(){
    return point;
}

bool BufferManager::pointIsAtEnd(){
    return (point == end);
}

char* BufferManager::getBuffer(){
    return buffer;
}

void BufferManager::toggleOverstrike(){
    overstrike = !overstrike;
}

bool BufferManager::getOverstrike(){
    return overstrike;
}

int BufferManager::getEnd(){
    return end;
}





