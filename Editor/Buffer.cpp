//
//  Buffer.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/7/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#include "Buffer.h"

Buffer::Buffer(){
    point = gapL = gapR = 0;
    buffer = createBuffer();
}

char* Buffer::createBuffer(){
    char *buffer = new char[MAX];
    return buffer;
}

void Buffer::setPointA(int location){
    point = location;
}

void Buffer::setPointR(int count){
    point += count;
}

int Buffer::getPoint(){
    return point;
}

void Buffer::addToTheGap(){                             //add to the gap from the end
    int count = 10000;
    if (count + end > MAX)
        cout<<"reached max characters"<<endl;
    int numToMove = end - gapL;
    for (int i = 0; i < numToMove; i++) {
        buffer[end - i] = buffer[end - i - 1];
    }
    end += count;
    gapL += count;
}

int Buffer::gapSize(){
    /*
    if (gapSize() < count) {
        addToTheGap();
    }
     */
    return gapR-gapL;
}
void Buffer::shiftGap(int count, bool toTheLeft){       //shift gap
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

void Buffer::checkGap(){
    if (point != gapL && point != gapR) {               //shift the gap if needed
        bool toTheLeft = (point < gapL);                //determine if point is to the left or right of the gap
        int count = toTheLeft ? gapL-point : point-gapR;//how many to shift
        shiftGap(count, toTheLeft);
    }
}

void Buffer::insert(string str){
    checkGap();                                         //check the gap
    
    bool toTheLeft = (point == gapL);
    if (toTheLeft)                                      //if point is to the left
        for (int i = 0; i < str.length(); i++) {            //insert char and update
            buffer[point] = str[i];
            point++; gapL++;
        }
    else                                                //if point is to the right
        for (int i = (int)str.length()-1; i >=0; i++) { //insert char and update
            point--; gapR--;
            buffer[point] = str[i];
        }
    
}

void Buffer::myDelete(int count){
    checkGap();                                         //check the gap
    if (count < 0)
        point = gapL = gapL + count;                    //delete by moving markers
    else
        point = gapR = gapR + count;
}

void Buffer::searchF(){
    
}
