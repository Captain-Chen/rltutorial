#include <ncurses.h>

#ifndef SCREEN_H
#define SCREEN_H

class Screen{
	int _height, _width;
public:
	// initialize ncurses library
	Screen();
	// clear ncurses
	~Screen();
	// print message to screen
	void add(const char *message);
	// get screen height and width
	int height();
	int width();
};

#endif
