//
//  main.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/3/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//
/*
 g++ -o edit main.cpp BufferManager.cpp Display.cpp -lcurses
 this is a text editor that uses that gap method. It supports entering text, delete(fn-delete) and backspace, moving with arrows and ctrl key combinations, overstrike mode, and search forward from the curser
 (supports wildcard ? matches one character and * matches zero or more characters)
 
 */

#include <iostream>
#include "curses.h"
#include "BufferManager.h"
#include "Display.h"
using namespace std;

#define KEY_BACKSPACE	127		/* backspace key    delete to left */
#define KEY_SEARCH_F    6       /* ctrl F key       search forward */
#define KEY_ESC         27      /* escape key       exit the search */
#define KEY_OVERSTRIKE  1       /* ctrl A key       overstrike toggle */
#define KEY_HOME        2       /* ctrl B key       go to beginning of line */
#define KEY_END         5       /* ctrl E key       go to the end of the line */
#define KEY_VERY_HOME   11      /* ctrl K key       go to the very end*/
#define KEY_VERY_END    12      /* ctrl L key       go to the very beginning */
#define KEY_QUIT        24      /* ctrl X key       quit the editor */


void run(BufferManager *buffer, Display *display){
    int input = 0;
    while (input != KEY_QUIT) {
        input = getch();
        int y,x;
        getyx(stdscr, y, x);
        switch (input) {
            case KEY_BACKSPACE:
                buffer->myDelete(-1);
                break;
            case KEY_DC:// fn del
                if (x >= 0)
                    buffer->myDelete(1);
                break;
            case KEY_LEFT:
                buffer->setPointR(-1);
                break;
            case KEY_RIGHT:
                buffer->setPointR(1);
                break;
            case KEY_UP:{
                position last;
                do {
                    buffer->setPointR(-1);
                    last = buffer->getCurserPosition(COLS, LINES);
                } while (!(last.x ==0 && last.y == 0) &&
                         ((last.x < x && last.y == y) ||
                         (last.x > x && last.y == y-1)));
                break;}
            case KEY_DOWN:{
                position last;
                do {
                    buffer->setPointR(1);
                    last = buffer->getCurserPosition(COLS, LINES);
                } while (!buffer->pointIsAtEnd() &&
                       ((last.x > x && last.y == y) ||
                        (last.x < x && last.y == y+1)));
                if (last.y > y + 1) buffer->setPointR(-1);
                break;}
            case KEY_HOME:{//beginning of line  //ctrl B
                position last = buffer->getCurserPosition(COLS, LINES);
                while (last.x != 0) {
                    buffer->setPointR(-1);
                    last = buffer->getCurserPosition(COLS, LINES);}
                break;}
            case KEY_END:{//end of line //ctrl E
                position last = buffer->getCurserPosition(COLS, LINES);
                bool isAtEnd = false;
                while (last.y != y + 1 && !isAtEnd) {
                    buffer->setPointR(1);
                    last = buffer->getCurserPosition(COLS, LINES);
                    isAtEnd = buffer->getEnd() == buffer->getPoint();}
                if (!isAtEnd) buffer->setPointR(-1);
                break;}
            case KEY_VERY_HOME://very beggining ctrl K
                buffer->setPointA(0, false);
                break;
            case KEY_VERY_END://very end ctrl  L
                buffer->setPointA(buffer->getEnd(), true);
                break;
            case KEY_OVERSTRIKE://insert/overstrike toggle
                buffer->toggleOverstrike();
                break;
            case KEY_SEARCH_F://ctr F, prompt on the status line to search
                display->startSearch(buffer->getPoint());
                break;
            case KEY_ESC://escape search
                display->stopSearch();
                break;
            default:
                if ((input > 31 && input < 127) || input == 10){
                    char c = (char)input;
                    string str = string(&c, 1);
                    if (display->isSearch)display->insert(str);
                    else buffer->insert(str);}
                break;
        }
            display->redisplay(buffer);
    }
}

int main(int argc, const char * argv[])
{
    //create buffer and display
    BufferManager *buffer = new BufferManager();
    Display *display = new Display(buffer);
    
    //setup curses
    initscr();
    noecho();
    keypad(stdscr, true);
    cbreak();
    
    //display initial status
    display->redisplay(buffer);
    
    //run
    run(buffer, display);
    
    //finish
    endwin();
    return 0;
}


