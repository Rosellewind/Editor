//
//  Buffer.h
//  Editor
//
//  Created by Roselle Milvich on 10/7/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//

#ifndef __Editor__Buffer__
#define __Editor__Buffer__

#include <iostream>
#include <string>
#include "curses.h"

using namespace std;

class Buffer{
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
    
public:
    char *buffer;/////////
    Buffer();
    void setPointA(int index);
    void setPointR(int count);
    int getPoint();
    void insert(string str);
    void myDelete(int count);
    void searchF();
    ~Buffer();
};




#endif /* defined(__Editor__Buffer__) */
