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
    point = gapL = gapR = end = overstrike = 0;
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
    point = location;
    bool pastTheGap = (point > gapL);                       //adjust if crosses the gap
    if (pastTheGap) point += gapSize();
}

void BufferManager::setPointR(int count){                   //set the point relative
    int total = point + count;
    bool crossesTheGap = ((point <= gapL && total > gapL) ||//adjust if crosses the gap
                          (point >= gapR && total < gapR));
    point += count;
    if (crossesTheGap) {
        if (count < 0) point -= (gapSize() - 1);
        else point += gapSize() - 1;
    }
}

int BufferManager::getPoint(){
    return point;
}

void BufferManager::addToTheGap(){                             //add to the gap from the end
    
    int count = 2000;
    if (count + end > MAX)
        cout<<"reached max characters"<<endl;
    int numToMove = end - gapL;
    for (int i = 0; i < numToMove; i++) {
        buffer[end - i] = buffer[end - i - 1];
    }
    end += count;
    gapR += count;
}

int BufferManager::gapSize(){
    return gapR-gapL;
}
void BufferManager::shiftGap(int count, bool toTheLeft){       //shift gap
    if (toTheLeft)
        for (int i = 0; i < count; i++) {
            gapR--; gapL--;
            buffer[gapR] = buffer[gapL];
        }
    else
        for (int i = 0; i < count; i++) {
            buffer[gapL] = buffer[gapR];
            gapR++; gapL++;
        }
}

void BufferManager::checkGap(){
    if (point > end) point = end;
    if (point != gapL && point != gapR) {               //shift the gap if needed
        bool toTheLeft = (point < gapL);                //determine if point is to the left or right of the gap
        int count = toTheLeft ? gapL-point : point-gapR;//how many to shift
        shiftGap(count, toTheLeft);
    }
}

void BufferManager::insert(string str){
    if (gapSize() < str.length()) addToTheGap();
    checkGap();                                         //check the gap
    
    bool toTheLeft = (point == gapL);
    if (toTheLeft)                                      //if point is to the left
        for (int i = 0; i < str.length(); i++) {            //insert char and update
            buffer[point] = str[i];
            point++; gapL++;
        }
    else                                                //if point is to the right
        for (int i = (int)str.length()-1; i >=0; i--) { //insert char and update
            buffer[point] = str[i];
            point--; gapR--;
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
    "   gapR:" + intToString(gapR) + "   end:" + intToString(end) + "   gapSize:" + intToString(gapSize());
    return str;
}

int BufferManager::getLine(int cols){
    bool pastTheGap = (point > gapL);           //adjust if crosses the gap
    if (pastTheGap) return (point - gapSize() + 1)/cols;
    else return point/cols;
}

int BufferManager::getCol(int cols){
    bool pastTheGap = (point > gapL);           //adjust if crosses the gap
    if (pastTheGap) return (point - gapSize() + 1) % cols;
    else return point % cols;
}






















