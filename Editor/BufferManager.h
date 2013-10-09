//
//  BufferManager.h
//  Editor
//
//  Created by Roselle Milvich on 10/8/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#ifndef __Editor__BufferManager__
#define __Editor__BufferManager__

#include <iostream>
#include <string>
#include "curses.h"

using namespace std;

class BufferManager{
    int point;
    int gapL;
    int gapR;
    int end;
    static const int MAX = INT32_MAX;
    char* createBuffer();
    void checkGap();
    int gapSize();
    void shiftGap(int count, bool toTheLeft);
    void addToTheGap();
    char *buffer;
    bool overstrike;
    
public:
    BufferManager();
    string intToString(int i);
    void setPointA(int index);
    void setPointR(int count);
    int getPoint();
    void insert(string str);
    void myDelete(int count);
    void searchF();
    string leftString();
    string rightString();
    string varString();
    int getLine(int cols);
    int getCol(int cols);
};
#endif /* defined(__Editor__BufferManager__) */
