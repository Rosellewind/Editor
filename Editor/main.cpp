//
//  main.cpp
//  Editor
//
//  Created by Roselle Milvich on 10/3/13.
//  Copyright (c) 2013 Roselle Milvich. All rights reserved.
//


#include <iostream>
#include <sstream>//////////
#include <stdlib.h>
#include "ncurses.h"
#include "BufferManager.h"
#include "Display.h"
using namespace std;
/*
/*
#define KEY_CTRL_F      0006       /* control F key */
#define KEY_ESC         0027      /* escape key */
#define KEY_CTRL_O      0015      /* control O key */
#define KEY_CTRL_END    0117   /* control end key */
#define KEY_CTRL_HOME   0119  /* control home key */
#define KEY_BACK        0127	/* backspace key */
#define KEY_DELETE      8		/* delete key */
#define KEY_LEFT_ARROW	0260		/* left-arrow key */
#define KEY_RIGHT_ARROW 0261		/* right-arrow key */
#define KEY_UP_ARROW    259		/* up-arrow key */
#define KEY_DOWN_ARROW	258		/* down-arrow key */
#define KEY_RETURN      10		/* enter/send key */
#define KEY_HOME        0071		/* home key */
#define KEY_END         0079		/* end key */
#define KEY_EXIT        551		/* exit key */
*/

void run(BufferManager *buffer, Display *display){
    wchar_t input = 0;
//    int input = 0;
    while (input != 'q') {
        input = getch();
//        buffer->insert(intToString(input));
  //      input = getch();
        int y,x;
        getyx(stdscr, y, x);
        switch (input) {
            case KEY_BACK:
                    buffer->myDelete(-1);
                break;
            case KEY_DELETE:
                if (x >= 0)
                    buffer->myDelete(1);
                break;
            case KEY_LEFT_ARROW:
                    buffer->setPointR(-1);
                break;
            case KEY_RIGHT_ARROW:
                    buffer->setPointR(1);
                break;
            case KEY_UP_ARROW:{
                position last;
                do {
                    buffer->setPointR(-1);
                    last = buffer->getPosition(COLS, LINES);
                } while (!(last.x ==0 && last.y == 0) &&
                         ((last.x < x && last.y == y) ||
                         (last.x > x && last.y == y-1)));
                break;}
            case KEY_DOWN_ARROW:{
                position last;
                do {
                    buffer->setPointR(1);
                    last = buffer->getPosition(COLS, LINES);
                } while (!buffer->pointIsAtEnd() &&
                       ((last.x > x && last.y == y) ||
                        (last.x < x && last.y == y+1)));
                if (last.y > y + 1) buffer->setPointR(-1);
                break;}
            case KEY_HOME:
                move(y,0);
                buffer->setPointR(x * -1);
                break;
            case KEY_END:
                move(y,COLS);
                buffer->setPointR(COLS - x);
                break;
                /*
                 //                   string str = ".." + intToString(last.y) + "," + intToString(last.x) + "..";
                 //                   addstr(str.c_str());
                 //                 getch();
                 */

            case KEY_CTRL_HOME:{///ctr home
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);}
                break;
            case KEY_CTRL_END:{////ctr end
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);}
                break;
            case KEY_CTRL_O:{//insert/overstrike toggle
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);}
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
    //create buffer
    BufferManager *buffer = new BufferManager();
    Display *display = new Display(buffer);
    
    //setup curses
    initscr();
    noecho();
    keypad(stdscr, true);
    cbreak();
    
    //display initial status
    display->redisplay(buffer);
    
    //define key codes
/*    define_key(key_ctrl_f, 300);
    define_key("\eO", 720);
    define_key(<#const char *#>, <#int#>)
*/    
    //run
    run(buffer, display);
    
    //finish
    endwin( );
    return 0;
}


