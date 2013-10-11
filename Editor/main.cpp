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
using namespace std;

//custom defined keys
#define KEY_CTRL_F  300     /* control F key */

//these keys are redefined to make them compatible on my mac
#define KEY_BACKSPACE	127		/* backspace key */
#define KEY_DC		8		/* delete-line key */
#define KEY_LEFT	260		/* left-arrow key */
#define KEY_RIGHT	261		/* right-arrow key */
#define KEY_UP		259		/* up-arrow key */
#define KEY_DOWN	258		/* down-arrow key */
#define KEY_ENTER	10		/* enter/send key */

//these keys are not available on my mac, but are included here for documentation
#define KEY_HOME	0406		/* home key */          //not avail on my mac
#define KEY_END		0550		/* end key */           //not avail on my mac
#define KEY_EXIT	0551		/* exit key */          //not avail on my mac

int stringSearch(const char *pattern, const char *text){
	int i, j, lengthOfPattern= (int)strlen(pattern),lengthOfText= (int)strlen(text);
    
    //increments i and j on matches, j is reset and i adjusted if no match
	for (i = 0, j = 0; j<strlen(pattern) && i<lengthOfText; i++,j++){
        if (pattern[j] == '*') {
            while (text[i+1] == pattern[j+1]) {i++; lengthOfPattern++;}
            i--; lengthOfPattern--; continue;
        }
        
        while(i<lengthOfText && pattern[j] != '?' && pattern[j] != '*' && text[i]!=pattern[j]){
            if ((strlen(pattern) > 0 && pattern[j-1] == '*')) {
                j--; lengthOfPattern++;}
            else {i=i-j+1; j = 0;}
        }
	}
	if(j==strlen(pattern)) return i-lengthOfPattern; else return -1;
}

string intToString(int i){//////////////
    string s;
    stringstream out;
    out << i;
    s = out.str();
    return s;
}
void redisplay(BufferManager *buffer, bool isSearch){
    
    //display text
    clear();
    string left = buffer->leftString();
    string right = buffer->rightString();
    addstr(left.c_str());
    addstr(right.c_str());
    
    position pos;
    if(isSearch){
        pos.y = LINES - 1;
        pos.x = 12;
    }
    else pos = buffer->getPosition(COLS, LINES);

    //display search
    if (isSearch){
        move(LINES-1, 0);
        string statusString = "Search for: ";
        addstr(statusString.c_str());
//        move(pos.y,pos.x);
        refresh();
    }
    
    //display status
    else{
        move(LINES-1, 0);
        string statusString = "Curser: " + intToString(pos.y) + "," + intToString(pos.x) + "  ";
        addstr(statusString.c_str());
        addstr(buffer->varString().c_str());
        move(pos.y,pos.x);
        refresh();
    }
}

void run(BufferManager *buffer){
    bool isSearch = false;
    int input = 0;
    while (input != 'q') {
        input = getch();
//        buffer->insert(intToString(input));
  //      input = getch();
        int y,x;
        getyx(stdscr, y, x);
        switch (input) {
            case KEY_BACKSPACE:
                    buffer->myDelete(-1);
                break;
            case KEY_DC:
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
                    last = buffer->getPosition(COLS, LINES);
                } while (!(last.x ==0 && last.y == 0) &&
                         ((last.x < x && last.y == y) ||
                         (last.x > x && last.y == y-1)));
                break;}
            case KEY_DOWN:{
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

             case 119:{///ctr home
                 int y,x;
                 getyx(stdscr, y, x);
                 move(y,COLS);}
                 break;
             case 117:{////ctr end
                 int y,x;
                 getyx(stdscr, y, x);
                 move(y,COLS);}
                 break;
             case 15:{//insert/overstrike toggle
                 int y,x;
                 getyx(stdscr, y, x);
                 move(y,COLS);}
                 break;
             case 6:{//ctr F, prompt on the status line to search
                 isSearch = true;
                 int y,x;
                 getyx(stdscr, y, x);
                 move(y,COLS);}
                 break;
            case KEY_EXIT:{//escape search
                ;}
                break;
            default:
                char c = (char)input;
                string str = string(&c, 1);
                buffer->insert(str);
                break;
        }
            redisplay(buffer, isSearch);
    }
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
    
    //display initial status
    redisplay(buffer, false);
    
    //define key codes
/*    define_key(key_ctrl_f, 300);
    define_key("\eO", 720);
    define_key(<#const char *#>, <#int#>)
*/    
    //run
    run(buffer);
    
    //finish
    endwin( );
    return 0;
}


