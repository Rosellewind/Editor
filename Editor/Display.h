//
//  Display.h
//  Editor
//
//  Created by Roselle Milvich on 10/10/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#ifndef __Editor__Display__
#define __Editor__Display__

#include <iostream>
#include "curses.h"
#include <string>
#include "BufferManager.h"

class Display {
    string searchWord;
    BufferManager *buffer;
    string intToString(int i);
    int searchPoint;
    bool displayResult;
    bool resultTrue;
    
public:
    bool isSearch;
    void startSearch(int point);
    void stopSearch();
    Display(BufferManager *bufferManager);
    void insert(string str);
    void redisplay(BufferManager *buffer);
};

#endif /* defined(__Editor__Display__) */
