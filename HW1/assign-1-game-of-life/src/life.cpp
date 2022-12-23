/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

/**
 * Function: initFileGrid
 * ----------------------
 * Init the grid by loading a starting configuration from a file.
 */
void initFileGrid(Grid<int>& grid, ifstream& fs) {
    string line;
    int row = 0, col = 0;
    int rIndex = 0;
    while (getline(fs, line)) {
        if (line[0] == '#') {
            continue;
        } else if (row == 0) {
            row = stoi(line);
        } else if (col == 0) {
            col = stoi(line);
        } else {
            if (rIndex == 0) {
                grid.resize(row, col);
                grid.fill(0);
            }
            for (size_t c=0; c<line.size(); c++) {
                if(line[c] == 'X') {
                    grid[rIndex][c] = 1;
                }
            }
            rIndex++;
        }
    }
}

/**
 * Function: initRandomGrid
 * ------------------------
 * Init the grid randomly.
 */
void initRandomGrid(Grid<int>& grid) {
    int row = randomInteger(40, 60+1);
    int col = randomInteger(40, 60+1);
    grid.resize(row, col);
    for (int r=0; r<row; r++) {
        for (int c=0; c<col; c++) {
            if (randomBool()) {
                grid[r][c] = randomInteger(1, kMaxAge+1);
            } else {
                grid[r][c] = 0;
            }
        }
    }
}

/**
 * Function: init
 * --------------
 * Init the grid
 */
void init(Grid<int>& grid) {
    cout << "You can start your colony with random cells or read from a prepared file." << endl;
    ifstream fs;
    string fileName;

    while (true) {
        fileName = getLine("Enter name of colony file (or RETURN to seed randomly):");
        if (fileName.empty()) {
            break;
        } else {
            fs.open(fileName);
            if (fs) {
                break;
            } else {
                cout << "Unable to open the file named \"" << fileName <<"\".  Please select another file." << endl;
            }
        }
    }

    if (fileName.empty()) {
        initRandomGrid(grid);
    } else {
        initFileGrid(grid, fs);
    }
}

void drawGrid(Grid<int>& grid, LifeDisplay& display) {
    display.setDimensions(grid.numRows(), grid.numCols());
    for (int r=0; r<grid.numRows(); r++) {
        for (int c=0; c<grid.numCols(); c++) {
            display.drawCellAt(r, c, grid[r][c]);
        }
    }
}


Grid<int> getNextGrid(const Grid<int>& grid) {
    int row = grid.numRows();
    int col = grid.numCols();
    Grid<int> nextGrid(row, col);
    for (int r=0; r<row; r++) {
        for (int c=0; c<col; c++) {
            int neighborCnt = 0;
            for (int k=0; k<dirLen; k++) {
                int neighbor_r = r + dr[k];
                int neighbor_c = c + dc[k];
                if (grid.inBounds(neighbor_r, neighbor_c) &&
                    grid[neighbor_r][neighbor_c] > 0) {
                    neighborCnt++;
                }
            }

            switch (neighborCnt) {
                case 0:
                case 1:
                    nextGrid[r][c] = 0;
                    break;
                
                case 2:
                    nextGrid[r][c] = grid[r][c];
                    break;

                case 3:
                    nextGrid[r][c] = 1;
                    break;

                default:
                    nextGrid[r][c] = 0;
                    break;
            }
        }
    }
    return nextGrid;
}

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");
    welcome();

    Grid<int> grid;
    init(grid);
    drawGrid(grid, display);
    display.repaint();
    display.printBoard();

    return 0;
}