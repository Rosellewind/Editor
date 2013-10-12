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
    string intToString(int i);
    bool crossesTheGap(int count);
    void addToTheGap();
    int gapSize();
    void shiftGap(int count, bool toTheLeft);
    void checkGap();
    
public:
    BufferManager();
    void setPointA(int location, bool adjust);
    void setPointR(int count);
    void insert(string str);
    void myDelete(int count);
    int searchF(const char *pattern, int searchPoint);

    string bufferString();
    string varString();
    
    position getCurserPosition(int cols, int rows);
    int getPoint();
    bool pointIsAtEnd();
    char* getBuffer();
    void toggleOverstrike();
    bool getOverstrike();
    int getEnd();
};
#endif /* defined(__Editor__BufferManager__) */
