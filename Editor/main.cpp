//
//  main.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/3/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//


#include <iostream>
#include "curses.h"
#include "BufferManager.h"
#include "Display.h"
using namespace std;

#define KEY_BACKSPACE	127		/* backspace key */
#define KEY_CTRL_F      6       /* control F key */
#define KEY_ESC         27      /* escape key */
#define KEY_CTRL_A      1       /* control O key */
#define KEY_HOME        2       /* ctrl B */
#define KEY_END         5       /* ctrl E */
#define KEY_CTRL_END    12      /* ctrl L */
#define KEY_CTRL_HOME   11      /* ctrl  K */
#define KEY_QUIT        24      /* ctrl  X */


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
            case KEY_CTRL_HOME://very beggining ctrl K
                buffer->setPointA(0);
                break;
            case KEY_CTRL_END://very end ctrl  L
                buffer->setPointA(buffer->getEnd());
                break;
            case KEY_CTRL_A://insert/overstrike toggle
                buffer->toggleOverstrike();
                break;
            case KEY_CTRL_F://ctr F, prompt on the status line to search
                display->startSearch(buffer->getPoint());
                break;
            case KEY_ESC://escape search
                display->stopSearch();
                break;
            default:
                char c = (char)input;
                string str = string(&c, 1);
                if (display->isSearch)display->insert(str);
                else buffer->insert(str);
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


