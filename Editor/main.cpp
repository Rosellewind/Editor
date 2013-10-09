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
#include "curses.h"
#include "BufferManager.h"
using namespace std;

//these keys are redefined to make them compatible on my mac
#define KEY_BACKSPACE	127		/* backspace key */
#define KEY_DC		8		/* delete-line key */
#define KEY_LEFT	260		/* left-arrow key */
#define KEY_RIGHT	261		/* right-arrow key */
#define KEY_UP		259		/* up-arrow key */
#define KEY_DOWN	258		/* down-arrow key */

//these keys are not available on my mac, but are included here for documentation
#define KEY_HOME	0406		/* home key */          //not avail on my mac
#define KEY_END		0550		/* end key */           //not avail on my mac
#define KEY_EXIT	0551		/* exit key */          //not avail on my mac


string intToString(int i){//////////////
    string s;
    stringstream out;
    out << i;
    s = out.str();
    return s;
}
void redisplay(BufferManager *buffer){

    
    clear();
    string left = buffer->leftString();
    string right = buffer->rightString();
    addstr(left.c_str());
    addstr(right.c_str());
    move(buffer->getLine(COLS), buffer->getCol(COLS));
    int y,x;
    getyx(stdscr, y, x);

    move(LINES-1, 0);
    string statusString = "Curser: " + intToString(y) + "," + intToString(x) + "   ";
    addstr(statusString.c_str());
    addstr(buffer->varString().c_str());
    move(y,x);
    refresh();
    
}

int main(int argc, const char * argv[])
{
    //create buffer
    BufferManager *buffer = new BufferManager();

    //setup curses
    initscr();
    noecho();
    keypad(stdscr, true);
    cbreak();
    
    //display info
    redisplay(buffer);
    
    //loop
    int input = 0;
    bool updateDisplay = false;
    while (input != 'q') {
        input = getch();
//        buffer->insert(intToString(input));
        switch (input) {
            case 'q':
                break;
            case KEY_BACKSPACE:
                buffer->myDelete(-1);
                updateDisplay = true;
                break;
            case KEY_DC:
                buffer->myDelete(1);
                updateDisplay = true;
                break;
            case KEY_LEFT:{
                int y,x;
                getyx(stdscr, y, x);
                move(y,x-1);
                buffer->setPointR(-1);
                updateDisplay = true;//////
                break;}
            case KEY_RIGHT:{
                int y,x;
                getyx(stdscr, y, x);
                move(y,x+1);
                buffer->setPointR(1);
                updateDisplay = true;/////////
                break;}
            case KEY_UP:{
                int y,x;
                getyx(stdscr, y, x);
                move(y-1,x);
                buffer->setPointR(-1 * COLS);
                break;}
            case KEY_DOWN:{
                int y,x;
                getyx(stdscr, y, x);
                move(y+1,x);
                buffer->setPointR(COLS);
                break;}
            case KEY_HOME:{
                int y,x;
                getyx(stdscr, y, x);
                move(y,0);
                buffer->setPointR(x * -1);
                break;}
            case KEY_END:{
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);
                buffer->setPointR(COLS - x);
                break;}
                /*
            case KEY_END:{///ctr home
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);}
                break;
            case KEY_END:{////ctr end
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);}
                break;
            case KEY_END:{//insert/overstrike toggle
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);}
                break;
            case KEY_END:{//ctr F, prompt on the status line to search
                int y,x;
                getyx(stdscr, y, x);
                move(y,COLS);}
                break;
                 */
            case KEY_EXIT:{//escape search
                ;}
                break;
            default:
                char c = (char)input;
                string str = string(&c, 1);
                buffer->insert(str);
                updateDisplay = true;
                break;
        }
        if (updateDisplay){
            redisplay(buffer);
            updateDisplay = false;
        }
    }
    
    //finish
    refresh( );
    endwin( );
	
    return 0;
}


