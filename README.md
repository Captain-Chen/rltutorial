# What is this?
A simple roguelike written in C++. Renders using the ncurses library.

## Gameplay Examples
| ![moving](img/rl_movement.gif) |
|---|
|*Basic movement in all 8 directions*|

| ![doors](img/rl_doors.gif) | ![items](img/rl_items.gif)  |
|---|---|
| *Opening and Closing Doors* | *Picking up and Dropping Items* |

# To compile the application:
1. `git clone https://github.com/Captain-Chen/rltutorial.git`
2. Install NCurses and ensure the library is properly linked to your compiler of choice.
3. Change into the directory and run `g++ *.cpp -lcurses -o rltutorial`

# Requirements
* [Ncurses](https://invisible-island.net/ncurses/#download_ncurses)
* A C++ compiler