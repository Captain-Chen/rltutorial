#include <ncurses.h>
#include "Screen.h"

// map variable
const int MAP_HEIGHT = 11;
const int MAP_WIDTH = 20;
int map[MAP_HEIGHT][MAP_WIDTH] = {
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// map functions
void initMap();
void drawTile();
bool isPassable(int mapX, int mapY);
void interactDoor(char action);

// player variables
int nPlayerX;
int nPlayerY;

struct TILE_TYPE{
	char nCharacter; // ASCII character
	short nColorCode; // Colour code
	bool bPassable; // Can you walk on this tile?
};

TILE_TYPE sTileIndex[] = {
	{'.', 7, true},   // 0 TILE_ROCKFLOOR
	{'#', 7, false},  // 1 TILE_WALL
	{'+', 6, false},  // 2 TILE_CLOSEDDOOR
	{'/', 6, true}	  // 3 TILE_OPENDOOR
};

const int TILE_ROCKFLOOR = 0;
const int TILE_WALL = 1;
const int TILE_CLOSEDDOOR = 2;
const int TILE_OPENDOOR = 3;

int main(void)
{
	// initialize ncurses
	Screen scr;
	scr.add("Welcome to game.\nPress any key to continue. Except 'q', that will quit the game.");
	int ch = getch();

	// initialize player character and position
	nPlayerX = 10; 
	nPlayerY = 10;
	int nDeltaX, nDeltaY;
	char player = '@';

	// colour definition
	start_color();
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);

	// main game loop
	while(true){
		// clear and add player character to screen; draw the map
		// map is drawn first then the player character is drawn on top
		clear();
		drawTile();
		attron(COLOR_PAIR(6));
		mvaddch(nPlayerY, nPlayerX, player);
		// reset nDeltaX and nDeltaY
		nDeltaX = nDeltaY = 0;
		attroff(COLOR_PAIR(6));
		// check for user input again
		ch = getch();
		switch(ch){
			// upper-left
			case 'y':
				nDeltaX = -1;
				nDeltaY = -1;
				break;
			// upper-right
			case 'u':
				nDeltaX = 1;
				nDeltaY = -1;
				break;
			// left
			case 'h':
				nDeltaX = -1;
				nDeltaY = 0;
				break;
			// down
			case 'j':
				nDeltaX = 0;
				nDeltaY = 1;
				break;
			// up
			case 'k':
				nDeltaX = 0;
				nDeltaY = -1;
				break;
			// right
			case 'l':
				nDeltaX = 1;
				nDeltaY = 0;
				break;
			// bottom-left
			case 'b':
				nDeltaX = -1;
				nDeltaY = 1;
				break;
			// bottom-right
			case 'n':
				nDeltaX = 1;
				nDeltaY = 1;
				break;
			// open door
			case 'o':
				interactDoor('o');
				break;
			// close door
			case 'c':
				interactDoor('c');
				break;
			case 'q':
				return 0;
			default:
				break;
		}
		if(isPassable(nPlayerX + nDeltaX, nPlayerY + nDeltaY)){
			nPlayerX += nDeltaX;
			nPlayerY += nDeltaY;
		}
	}
	return 0;
}

void drawTile(){
	for(int mapy = 0; mapy < MAP_HEIGHT; mapy++){
		for(int mapx = 0; mapx < MAP_WIDTH; mapx++){
			// get tile type
			int nType = map[mapy][mapx];

			attron(COLOR_PAIR(sTileIndex[nType].nColorCode));
			mvaddch(mapy, mapx, sTileIndex[nType].nCharacter);
			attroff(COLOR_PAIR(sTileIndex[nType].nColorCode));
		}
	}
}

// function: check if a tile is passable
bool isPassable(int nMapX, int nMapY){
	// check if coordinates are valid; i.e. can't walk off the map
	if(nMapX < 0 || nMapX >= MAP_WIDTH || nMapY < 0 || nMapY >= MAP_HEIGHT)
		return false;

	// grab tile information from map array
	int nTileValue = map[nMapY][nMapX];
	return sTileIndex[nTileValue].bPassable;
}

void interactDoor(char action){
	mvprintw(MAP_HEIGHT + 1, 0, "What direction?");
	int ch = getch();
	int nDeltaX, nDeltaY;
	int nTileValue;

	switch(ch){
	// upper-left
		case 'y':
			nDeltaX = -1;
			nDeltaY = -1;
			break;
		// upper-right
		case 'u':
			nDeltaX = 1;
			nDeltaY = -1;
			break;
		// left
		case 'h':
			nDeltaX = -1;
			nDeltaY = 0;
			break;
		// down
		case 'j':
			nDeltaX = 0;
			nDeltaY = 1;
			break;
		// up
		case 'k':
			nDeltaX = 0;
			nDeltaY = -1;
			break;
		// right
		case 'l':
			nDeltaX = 1;
			nDeltaY = 0;
			break;
		// bottom-left
		case 'b':
			nDeltaX = -1;
			nDeltaY = 1;
			break;
		// bottom-right
		case 'n':
			nDeltaX = 1;
			nDeltaY = 1;
			break;
		default:
			clear();
			return;
	}

	nTileValue = map[nPlayerY + nDeltaY][nPlayerX + nDeltaX];
	if(action == 'o' && nTileValue == TILE_CLOSEDDOOR){
		map[nPlayerY + nDeltaY][nPlayerX + nDeltaX] = TILE_OPENDOOR;
	}
	else if(action == 'o' && nTileValue == TILE_OPENDOOR)
	{
		mvprintw(MAP_HEIGHT + 1, 0, "The door is already open!");
	}
	
	if(action == 'c' && nTileValue == TILE_OPENDOOR){
		map[nPlayerY + nDeltaY][nPlayerX + nDeltaX] = TILE_CLOSEDDOOR;
	}
	else if(action == 'c' && nTileValue == TILE_CLOSEDDOOR)
	{
		mvprintw(MAP_HEIGHT + 1, 0, "The door is already closed!");
	}
	getch();
}
