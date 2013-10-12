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

struct position {
    int x;
    int y;
};

class BufferManager{
    static const int MAX_DOCUMENT_LENGTH = INT32_MAX;
    static const int GAP_BLOCK_SIZE = 2000;
    char *buffer;
    int point;
    int gapL;
    int gapR;
    int end;
    bool overstrike;
    bool usingEndMarker;
    
    char* createBuffer();
    bool crossesTheGap(int count);
    int crossesCarriageReturn(int count);
    void addToTheGap();
    int gapSize();
    void shiftGap(int count, bool toTheLeft);
    void checkGap();

    
public:
    BufferManager();
    string intToString(int i);
    void setPointA(int index);
    void setPointR(int count);
    int getPoint();
    bool pointIsAtEnd();
    void insert(string str);
    void myDelete(int count);
    void searchF();
    string leftString();
    string rightString();
    string varString();
    position getPosition(int cols, int rows);
    int getLine(int cols);
    int getCol(int cols);
    char* getBuffer();
    void toggleOverstrike();
    bool getOverstrike();
    int getEnd();
};
#endif /* defined(__Editor__BufferManager__) */
