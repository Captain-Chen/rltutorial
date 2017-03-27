#include <ncurses.h>
#include "Screen.h"

// map variable
const int map_sizeY = 11;
const int map_sizeX = 20;
int map[map_sizeY][map_sizeX] = {
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// initialize map info
const int TILE_FLOOR = 0;
const int TILE_WALL = 1;

// map functions
void initMap();
void drawMap();
bool isPassable(int mapX, int mapY);

int main(void)
{
	// initialize ncurses
	Screen scr;
	scr.add("Welcome to game.\nPress any key to continue. Except 'q', that will quit the game.");
	int ch = getch();

	// initialize player character and position
	int playerPosX = 10; 
	int playerPosY = 10;
	char player = '@';

	// main game loop
	while(true){
		// clear and add player character to screen; draw the map
		// map is drawn first then the player character is drawn on top
		clear();
		drawMap();
		mvaddch(playerPosY, playerPosX, player);

		// check for user input again
		ch = getch();
		switch(ch){
			// upper-left
			case 'y':
				if(isPassable(playerPosX - 1, playerPosY - 1)){
					playerPosX--;
					playerPosY--;
				}
				break;
			// upper-right
			case 'u':
				if(isPassable(playerPosX + 1, playerPosY - 1)){
					playerPosX++;
					playerPosY--;
				}
				break;
			// left
			case 'h':
				if(isPassable(playerPosX - 1, playerPosY)){
					playerPosX--;
				}
				break;
			// down
			case 'j':
				if(isPassable(playerPosX, playerPosY + 1)){
					playerPosY++;
				}
				break;
			// up
			case 'k':
				if(isPassable(playerPosX, playerPosY - 1)){
					playerPosY--;
				}
				break;
			// right
			case 'l':
				if(isPassable(playerPosX + 1, playerPosY)){
					playerPosX++;
				}
				break;
			// bottom-left
			case 'b':
				if(isPassable(playerPosX - 1, playerPosY + 1)){
					playerPosX--;
					playerPosY++;
				}
				break;
			// bottom-right
			case 'n':
				if(isPassable(playerPosX + 1, playerPosY + 1)){
					playerPosX++;
					playerPosY++;
				}
				break;
			case 'q':
				return 0;
			default:
				break;
		}
	}
	return 0;
}

// draw the map
void drawMap(){
	for(int mapy = 0; mapy < map_sizeY; mapy++){
		for(int mapx = 0; mapx < map_sizeX; mapx++){
				switch(map[mapy][mapx])
				{
					case TILE_FLOOR:
						mvaddch(mapy, mapx, '.');
						break;
					case TILE_WALL:
						mvaddch(mapy, mapx, '#');
						break;
				}
		}
	}
}

// function: check if a tile is passable
bool isPassable(int nMapX, int nMapY){
	// grab tile information from map
	int tileValue = map[nMapY][nMapX];

	// return true if tile is passable; otherwise return false
	if(tileValue == TILE_FLOOR)
		return true;

	return false;
}
