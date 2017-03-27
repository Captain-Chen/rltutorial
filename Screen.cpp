#include "Screen.h"

// initialize ncurses library
Screen::Screen(){
	initscr();
	clear();
	noecho(); // don't echo() while we call getch()
	cbreak();
	//raw(); // line buffering disabled
	keypad(stdscr, TRUE); // get special keys like F1, F2
	curs_set(0);
	// get screen dimensions
	getmaxyx(stdscr, _height, _width); // get boundaries of the main screen
}

// clear ncurses
Screen::~Screen(){
	endwin();
}

// print message to screen
void Screen::add(const char *message){
	printw(message);
}

// get screen height
int Screen::height(){
	return _height;
}

// get screen width
int Screen::width(){
	return _width;
}
