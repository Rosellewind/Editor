//
//  BufferManager.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/8/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#include "BufferManager.h"
#include <sstream>


BufferManager::BufferManager(){
    point = gapL = gapR = end = overstrike = usingEndMarker = 0;
    buffer = createBuffer();
}

string BufferManager::intToString(int i){
    string s;
    stringstream out;
    out << i;
    s = out.str();
    return s;
}

char* BufferManager::createBuffer(){
    char *buffer = new char[MAX];
    return buffer;
}

void BufferManager::setPointA(int location){                //set the point to location
    point = location;////////////
    bool pastTheGap = (point > gapL);                       //adjust if crosses the gap
    if (pastTheGap) point += gapSize();
}

void BufferManager::setPointR(int count){                   //set the point relative
    int total = point + count;
    bool crossesTheGap = false;
    if (point == gapL || point == gapL - 1)
        crossesTheGap = (count > 0);
    else if (point == gapR)
        crossesTheGap = (count < 0);
    else if((point < gapL && total >= gapR)||
         (point > gapR && total < gapL))
        crossesTheGap = true;
    if (crossesTheGap) {
        if (count < 0) point -= gapSize();
        else{
            if (point == gapL)
                point += gapSize();
            else point += gapSize();
        }
    }
    point += count;
}

int BufferManager::getPoint(){
    return point;
}

void BufferManager::addToTheGap(){                             //add to the gap from the end
    
    int count = 2000;
    if (count + end > MAX)
        cout<<"reached max characters"<<endl;
    int numToMove = end - gapR;///////gapL
    
    for (int i = 0; i < numToMove; i++) {
        buffer[end + count - i - 1] = buffer[end - i - 1];
    }
    end += count;
    gapR += count;
}

int BufferManager::gapSize(){
    return gapR-gapL;
}
void BufferManager::shiftGap(int count, bool toTheLeft){       //shift gap
    if (toTheLeft)
        if (gapL == gapR) {
            gapR = gapL = point;
        }
        else{
            for (int i = 0; i < count; i++) {
                gapR--; gapL--;
                buffer[gapR] = buffer[gapL];////////
            }
        }
    else
        for (int i = 0; i < count; i++) {/////+1ck
            buffer[gapL] = buffer[gapR];
            gapR++; gapL++;
        }
    point = gapL;
}

void BufferManager::checkGap(){
    if (point > end) point = end;
    if (point == end) usingEndMarker = true;
    else usingEndMarker = false;
    if (point != gapL && point != gapR) {               //shift the gap if needed
        if (gapL == gapR) {
            gapR = gapL = point;
        }
        else {
            bool toTheLeft = (point < gapL);                //determine if point is to the left or right of the gap
            int count = toTheLeft ? gapL-point: point-gapR;//how many to shift
            shiftGap(count, toTheLeft);
        }
    }
}

void BufferManager::insert(string str){
    checkGap();                                         //check the gap
    if (!usingEndMarker && gapSize() < str.length()) addToTheGap();
    for (int i = 0; i < str.length(); i++) {            //insert char and update
        buffer[point] = str[i];
        point++;
        if (usingEndMarker) {
            if (end == gapL)
                gapL++; gapR++;
            end++;
        }
        else gapL++;
    }
}

void BufferManager::myDelete(int count){
    checkGap();                                         //check the gap
    if (count < 0)
        point = gapL = gapL + count;                    //delete by moving markers
    else
        point = gapR = gapR + count;
}

void BufferManager::searchF(){
    
}

string BufferManager::leftString(){
    int sizeLeft = gapL;
    string str(buffer, sizeLeft);
    return str;
}

string BufferManager::rightString(){
    int sizeRight = end - gapR;
    string str(&buffer[gapR], sizeRight);
    return str;
}

string BufferManager::varString(){
    string str = "point:" + intToString(point) + "   gapL:" + intToString(gapL) +
    " gapR:" + intToString(gapR) + " end:" + intToString(end) + " gapSize:" + intToString(gapSize()) + " atPoint:" + intToString(buffer[point]);
    return str;
}

int BufferManager::getLine(int cols){
    bool pastTheGap = (point > gapL);           //adjust if crosses the gap
    if (pastTheGap) return (point - gapSize() + 1)/cols;
    else return point/cols;
}

int BufferManager::getCol(int cols){
    bool pastTheGap = (point > gapL);           //adjust if crosses the gap
    if (pastTheGap) return (point - gapSize() ) % cols;
    else return point % cols;
}






















