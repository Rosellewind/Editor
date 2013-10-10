//
//  BufferManager.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/8/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#include "BufferManager.h"
#include <sstream>

//private methods

char* BufferManager::createBuffer(){
    char *buffer = new char[MAX_DOCUMENT_LENGTH];
    return buffer;
}

bool BufferManager::crossesTheGap(int count){
    int total = point + count;
    bool cross = false;
    if (point == gapL || point == gapL - 1) cross = (count > 0);
    else if (point == gapR) cross = (count < 0);
    else if((point < gapL && total >= gapR)||
            (point > gapR && total < gapL)) cross = true;
    return cross;
}

int BufferManager::crossesCarriageReturn(int count){
    int acc = 0;
    if (count < 0){
        for (int i = count; i<=0; i++)//////////
            if (buffer[point + i] == 10) acc++;

    }
    else
        for (int i = 0; i < count; i++){
            if (buffer[point + i] == 10) acc++;
        }
    return acc;
}


void BufferManager::addToTheGap(){                             //add to the gap from the end

    int count = GAP_BLOCK_SIZE;
    if (count + end > MAX_DOCUMENT_LENGTH)
        cout<<"reached max characters"<<endl;
    int numToMove = end - gapR;
    
    for (int i = 0; i < numToMove; i++) {
        buffer[end + count - i - 1] = buffer[end - i - 1];
    }
    end += count; gapR += count;
}

int BufferManager::gapSize(){
    return gapR-gapL;
}
void BufferManager::shiftGap(int count, bool toTheLeft){       //shift gap
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

void BufferManager::checkGap(){
    if (point > end) point = end;
    if (point == end) usingEndMarker = true;
    else usingEndMarker = false;
    if (!usingEndMarker && point != gapL && point != gapR) {               //shift the gap if needed
        if (gapL == gapR) gapR = gapL = point;
        else {
            bool toTheLeft = (point < gapL);                //determine if point is to the left or right of the gap
            int count = toTheLeft ? gapL-point: point-gapR;//how many to shift
            shiftGap(count, toTheLeft);
        }
    }
}


//public methods
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

void BufferManager::setPointA(int location){                //set the point to location
    point = location;
    bool pastTheGap = (point > gapL);                       //adjust if crosses the gap
    if (pastTheGap) point += gapSize();
}
//setPointR just sets relative to buffer, does not reflect screen
void BufferManager::setPointR(int count){           //set the point relative
    int newPoint = point;
    if (crossesTheGap(count)) {
        if (count < 0) newPoint -= gapSize();          //going to the left
        else  newPoint += gapSize();
    }
    newPoint += count;
    if (newPoint >= 0) point = newPoint;
}

int BufferManager::getPoint(){
    return point;
}

void BufferManager::insert(string str){
    checkGap();                                         //check the gap
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
    string str = "point:" + intToString(point) + "  gapL:" + intToString(gapL) +
    "  gapR:" + intToString(gapR) + "  end:" + intToString(end) + "  gapSize:" + intToString(gapSize()) + "  atPoint:" + intToString(buffer[point]);
    return str;
}

position BufferManager::getPosition(int cols, int rows){
    position pos;
    pos.x = pos.y = 0;
    for (int i = 0; i < point; i++) {
        bool pastTheGap = (i == gapL);       //adjust if crosses the gap
        if (pastTheGap) i += gapSize();
        int c = buffer[i];
        if (c == 10){pos.x = 0; pos.y++;}
        else if (pos.x == cols) {pos.x = 1; pos.y++;}
        else {pos.x++;}
    }
    return pos;
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





//delete too far
//return
//search
//control















