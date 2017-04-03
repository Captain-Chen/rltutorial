#include <ncurses.h>
#include "Screen.h"

#include <string.h>

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

// define items array; mapping 2D array into a 1D array (so iteration is easier)
//int items[MAP_WIDTH * MAP_HEIGHT];
int items[MAP_HEIGHT][MAP_WIDTH]; // keep it consistent for now as we are using 2D arrays

int inventory[10];

// map functions
void initItems();
void drawMap();
void drawTile();
bool isPassable(int mapX, int mapY);
void interactDoor(char action);
void getCommand();
void dropCommand();

void showInventory();

// player variables
int nPlayerX, nPlayerY;

// new defined user type
struct TILE_TYPE{
	char nCharacter; // ASCII character
	short nColorCode; // Colour code
	bool bPassable; // Can you walk on this tile?
};

struct ITEM_TYPE{
	char nCharacter; // ASCII character code
	short nColorCode; // Colour code
	char *p_szName; // Name of the item
};

TILE_TYPE sTileIndex[] = {
	{'.', 7, true},   // 0 TILE_ROCKFLOOR
	{'#', 7, false},  // 1 TILE_WALL
	{'+', 6, false},  // 2 TILE_CLOSEDDOOR
	{'/', 6, true}	  // 3 TILE_OPENDOOR
};

ITEM_TYPE sItemIndex[] = {
	{' ', 7, "EMPTY"},			// 0 ITEM_EMPTY
	{(char)173, 6, "Potion"},	// 1 ITEM_POTION
	{'*', 7, "Rock"}			// 2 ITEM_ROCK
};

// tile types
const int TILE_ROCKFLOOR = 0;
const int TILE_WALL = 1;
const int TILE_CLOSEDDOOR = 2;
const int TILE_OPENDOOR = 3;

// item types
const int ITEM_EMPTY = 0;
const int ITEM_POTION = 1;
const int ITEM_ROCK = 2;

const int INVENTORY_SLOTS = sizeof(inventory) / sizeof(inventory[0]);

int main(void)
{
	// initialize ncurses
	Screen scr;
	scr.add("Welcome to game.\nPress any key to continue. Except 'q', that will quit the game.");
	int ch = getch();

	// initialize player character and position
	nPlayerX = MAP_WIDTH / 2; 
	nPlayerY = MAP_HEIGHT / 2; 
	int nDeltaX, nDeltaY;
	nDeltaX = nDeltaY = 0;

	char player = '@';

	// colour definition
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	initItems();
	// initialize inventory
	memset(inventory, ITEM_EMPTY, sizeof(inventory));
	
	// add stuff to item map
	items[5][9] = ITEM_ROCK;
	items[4][6] = ITEM_POTION;

	// main game loop
	while(true){
		// clear and add player character to screen; draw the map
		// map is drawn first then the player character is drawn on top
		clear();
		drawMap();
		showInventory();
		// set colour and draw player to screen
		attron(COLOR_PAIR(1));
			mvaddch(nPlayerY, nPlayerX, player);
		attroff(COLOR_PAIR(1));
		// reset deltaY and deltaX position
		nDeltaY = nDeltaX = 0;
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
				interactDoor(ch);
				break;
			// close door
			case 'c':
				interactDoor(ch);
				break;
			case 'q':
				return 0;
			case ',':
				getCommand();
				break;
			case 'D':
				dropCommand();
				break;
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

void initItems(){
	for(int y = 0; y < MAP_HEIGHT; y++){
		for(int x = 0; x < MAP_WIDTH; x++){
			items[y][x] = ITEM_EMPTY;
		}
	}
}

void showInventory(){
	mvaddstr(1, MAP_WIDTH + 2, "INVENTORY");		
	mvaddstr(2, MAP_WIDTH + 2, "---------");

	for(int i = 0; i < INVENTORY_SLOTS; i++){
		// get itemtype
		int nType = inventory[i];

		// draw to screen
		mvprintw(3+i, MAP_WIDTH + 2, sItemIndex[nType].p_szName);
	}
}

void drawTile(int x, int y){
	int nColor;
	int nType;
	char nCharacter;

	if(items[y][x] != ITEM_EMPTY)
	{
		// get tile value
		nType = items[y][x];
		nColor = sItemIndex[nType].nColorCode;
		nCharacter = sItemIndex[nType].nCharacter;
	}
	else
	{
		// get tile value
		nType = map[y][x];
		nColor = sTileIndex[nType].nColorCode;
		nCharacter = sTileIndex[nType].nCharacter;
	}
	// get color type then draw it to screen
	attron(COLOR_PAIR(nColor));
	mvaddch(y, x, nCharacter);
	attroff(COLOR_PAIR(nColor));
}

void drawMap(){
	for(int y = 0; y < MAP_HEIGHT; y++){
		for(int x = 0; x < MAP_WIDTH; x++){
			drawTile(x, y);
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
		mvprintw(MAP_HEIGHT + 3, 0, "The door is already open!");
		getch();
	}
	
	if(action == 'c' && nTileValue == TILE_OPENDOOR){
		map[nPlayerY + nDeltaY][nPlayerX + nDeltaX] = TILE_CLOSEDDOOR;
	}
	else if(action == 'c' && nTileValue == TILE_CLOSEDDOOR)
	{
		mvprintw(MAP_HEIGHT + 3, 0, "The door is already closed!");
		getch();
	}
}

void getCommand(){ 
	// check if there is an item at the player's current position
	if(items[nPlayerY][nPlayerX] == ITEM_EMPTY){ 
		return;
	}

	for(int i = 0; i < INVENTORY_SLOTS; i++){
		if(inventory[i] == ITEM_EMPTY){
			// copy value into inventory
			inventory[i] = items[nPlayerY][nPlayerX];
			// replace value of item to empty
			items[nPlayerY][nPlayerX] = ITEM_EMPTY;
			return;
		}
	}
	
	// no item slots available
	return;
}

void dropCommand(){
	mvprintw(MAP_HEIGHT + 1, 0, "Drop from which slot?");
	int ch = getch();
	
	// convert letters to numbers using off-set
	int nSlot = (char)ch - 'A'; 

	// check if this is a valid slot
	if(nSlot < 0 || nSlot >= INVENTORY_SLOTS){
		mvprintw(MAP_HEIGHT + 3, 0, "Invalid inventory slot");	
		getch();
	}
	// check if there is actually an item
	else if(inventory[nSlot] == ITEM_EMPTY)
	{
		mvprintw(MAP_HEIGHT + 3, 0, "Nothing in this item slot");
		getch();
	}
	// check if the player if there is room to drop this item
	else if(items[nPlayerY][nPlayerX] != ITEM_EMPTY)
	{
		mvprintw(MAP_HEIGHT + 3, 0, "There's no room to drop this item");
		getch();
	}
	else
	{
		// drop item onto ground
		items[nPlayerY][nPlayerX] = inventory[nSlot];
		inventory[nSlot] = ITEM_EMPTY;
	}
}

