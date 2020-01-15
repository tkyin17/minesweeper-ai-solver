//Written by Pieter Molenaar, Thien Nguyen, Amir Seifpour

#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <map>

#define ROWS 10
#define COLUMNS 10
#define NUMBOMBS 10

using namespace std;

const int NOBOMBS = (ROWS * COLUMNS) - NUMBOMBS; //number of spaces without bombs
int minesPlaced = 0;

//for storing x, y coordinates, tile values, and num of adjacent flags to that tile
class coord
{
	public:
		int value; //number of adj bombs value
		int row;
		int col;
		//int adjFlags;
		
		coord(int v, int r, int c)
		{
			value = v;
			row = r;
			col = c;
			//adjFlags = 0;
		}
};	
	
//for printing game boards current state
template <class T>
void printArray(T game[ROWS][COLUMNS], int flags)
{
	//cout << "Flags left : " << flags << endl;
	cout << "Number of Mines left:" << minesPlaced-flags << " | " << "Number of flags placed: " << flags << "\n\n";
	cout << "    ";
	for(int k = 0; k < COLUMNS; k++)
		cout << k % 10 << " ";
	cout << endl << "-------------------------------------------" << endl;
	for(int i = 0; i < ROWS; i++)
	{
		cout << i % 10  << " |" << " ";
		for(int j = 0; j < COLUMNS; j++)
		{
			cout << game[i][j] << " ";
		}
	cout << endl;
	}
	cout << endl << endl;
}

template <class T>
void printArray(T game[ROWS][COLUMNS])
{
	//cout << "Flags left : " << flags << endl;
	cout << "    ";
	for(int k = 0; k < COLUMNS; k++)
		cout << k % 10 << " ";
	cout << endl << "-------------------------------------------" << endl;
	for(int i = 0; i < ROWS; i++)
	{
		cout << i % 10  << " |" << " ";
		for(int j = 0; j < COLUMNS; j++)
		{
			cout << game[i][j] << " ";
		}
	cout << endl;
	}
	cout << endl << endl;
}


void addMines(int (&gameBoard)[ROWS][COLUMNS])
{
	srand(time(NULL));
	
	int bombRows[NUMBOMBS]; //bombs row indexes
	for(int i = 0; i < NUMBOMBS; i++)
	{
		bombRows[i] = rand() % ROWS;
		//cout << bombRows[i] << " ";
	}
	std::cout << endl;
	
	int bombCols[NUMBOMBS]; //bombs col indexes
	for(int i = 0; i < NUMBOMBS; i++)
	{
		bombCols[i] = rand() % COLUMNS;
		//cout << bombCols[i] << " ";
	}
	
	for(int i = 0; i < NUMBOMBS; i++)
	{
		if (gameBoard[bombRows[i]][bombCols[i]] != 9) {
			gameBoard[bombRows[i]][bombCols[i]] = 9;
			++minesPlaced;
		}
	}
}

void addNumbers(int (&gb)[ROWS][COLUMNS])
{
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			if(gb[i][j] == 9)
			{
				if(j > 0)//update left
				{
					if(gb[i][j-1] != 9) ++gb[i][j-1];
				}
				if(j < COLUMNS - 1)//update right
				{
					if(gb[i][j+1] != 9) ++gb[i][j+1];
				}
				if(i > 0)//update up
				{
					if(gb[i-1][j] != 9) ++ gb[i-1][j];
				}
				if(i < ROWS - 1)//update down
				{
					if(gb[i+1][j] != 9) ++ gb[i+1][j];
				}
				if(i > 0 && j > 0)//update up-left
				{
					if(gb[i-1][j-1] != 9) ++gb[i-1][j-1];
				}
				if(i > 0 && j < COLUMNS - 1)//update up-right
				{
					if(gb[i-1][j+1] != 9) ++gb[i-1][j+1];
				}
				if(i < ROWS - 1 && j > 0)//update down-left
				{
					if(gb[i+1][j-1] != 9) ++gb[i+1][j-1];
				}
				if(i < ROWS - 1 && j < COLUMNS - 1)//update down-right
				{
					if(gb[i+1][j+1] != 9) ++gb[i+1][j+1];
				}
			}
		}
	}
}

//uncover zeros and their neighbors                                                        y             x
vector<coord> uncoverZeroBFS(char (&pb)[ROWS][COLUMNS], int gbCopy[ROWS][COLUMNS], int startRow, int startCol)
{
	//use BFS to go through all adjacent zeros
	vector<coord> perimeter;
	int currentY, currentX;
	queue<int> rowY;
	queue<int> colX;
	rowY.push(startRow);
	colX.push(startCol);
	while(!rowY.empty() && !colX.empty())
	{
		currentY = rowY.front();
		currentX = colX.front();
		rowY.pop();
		colX.pop();
		if (gbCopy[currentY][currentX] == 0)
			pb[currentY][currentX] = '-';
		//else	
		//	pb[currentY][currentX] = '0' + gbCopy[currentY][currentX];
		
		if(currentX > 0)//uncover left
		{
			if((gbCopy[currentY][currentX-1] != 0 && gbCopy[currentY][currentX-1] != -1) && pb[currentY][currentX-1] == 'X') 
			{	
				if (gbCopy[currentY][currentX-1] != 9) {
					pb[currentY][currentX-1] = '0' + gbCopy[currentY][currentX-1];
					perimeter.push_back(coord(gbCopy[currentY][currentX-1], currentY, currentX-1)); //adds value to vector so we can parse later into list
				}
			}
		}
		if(currentX < COLUMNS - 1)//uncover right
		{
			if((gbCopy[currentY][currentX+1] != 0  && gbCopy[currentY][currentX+1] != -1)  && pb[currentY][currentX+1] == 'X') 
			{
				if (gbCopy[currentY][currentX+1] != 9) {
					pb[currentY][currentX+1] = '0' + gbCopy[currentY][currentX+1];
					perimeter.push_back(coord(gbCopy[currentY][currentX+1], currentY, currentX+1));
				}
			}
		}
		if(currentY > 0)//uncover up
		{
			if((gbCopy[currentY-1][currentX] != 0 && gbCopy[currentY-1][currentX] != -1) && pb[currentY-1][currentX] == 'X') 
			{
				if (gbCopy[currentY-1][currentX] != 9) {
					pb[currentY-1][currentX] = '0' + gbCopy[currentY-1][currentX];
					perimeter.push_back(coord(gbCopy[currentY-1][currentX], currentY-1, currentX));
				}
			}
		}
		if(currentY < ROWS - 1)//uncover down
		{
			if((gbCopy[currentY+1][currentX] != 0 && gbCopy[currentY+1][currentX] != -1) && pb[currentY+1][currentX] == 'X') 
			{	
				if (gbCopy[currentY+1][currentX] != 9) {
					pb[currentY+1][currentX] = '0' + gbCopy[currentY+1][currentX];
					perimeter.push_back(coord(gbCopy[currentY+1][currentX], currentY+1, currentX));
				}	
			}
		}
		if(currentY > 0 && currentX > 0)//uncover up-left
		{
			if((gbCopy[currentY-1][currentX-1] != 0 && gbCopy[currentY-1][currentX-1] != -1) && pb[currentY-1][currentX-1] == 'X') 
			{
				if (gbCopy[currentY-1][currentX-1] != 9) {
					pb[currentY-1][currentX-1] = '0' + gbCopy[currentY-1][currentX-1];
					perimeter.push_back(coord(gbCopy[currentY-1][currentX-1], currentY-1, currentX-1));
				}	
			}
		}
		if(currentY > 0 && currentX < COLUMNS - 1)//uncover up-right
		{
			if((gbCopy[currentY-1][currentX+1] != 0 && gbCopy[currentY-1][currentX+1] != -1) && pb[currentY-1][currentX+1] == 'X') 
			{
				if (gbCopy[currentY-1][currentX+1] != 9) {
					pb[currentY-1][currentX+1] = '0' + gbCopy[currentY-1][currentX+1];
					perimeter.push_back(coord(gbCopy[currentY-1][currentX+1], currentY-1, currentX+1));
				}
			}
		}
		if(currentY < ROWS - 1 && currentX > 0)//uncover down-left
		{
			if((gbCopy[currentY+1][currentX-1] != 0 && gbCopy[currentY+1][currentX-1] != -1) && pb[currentY+1][currentX-1] == 'X') 
			{
				if (gbCopy[currentY+1][currentX-1] != 9) {
					pb[currentY+1][currentX-1] = '0' + gbCopy[currentY+1][currentX-1];
					perimeter.push_back(coord(gbCopy[currentY+1][currentX-1], currentY+1, currentX-1));
				}
			}
		}
		if(currentY < ROWS - 1 && currentX < COLUMNS - 1)//uncover down-right
		{
			if((gbCopy[currentY+1][currentX+1] != 0 && gbCopy[currentY+1][currentX+1] != -1) && pb[currentY+1][currentX+1] == 'X') 
			{
				if (gbCopy[currentY+1][currentX+1] != 9) {
					pb[currentY+1][currentX+1]= '0' + gbCopy[currentY+1][currentX+1];
					perimeter.push_back(coord(gbCopy[currentY+1][currentX+1], currentY+1, currentX+1));
				}
			}
		}
		
		//update current as visited
		gbCopy[currentY][currentX] = -1;
		
		//enqueue neighbors
		if(currentY < ROWS - 1 && gbCopy[currentY+1][currentX] == 0)//enqueue down
		{
			rowY.push(currentY+1);
			colX.push(currentX);
		}	
		if(currentX > 0 && gbCopy[currentY][currentX-1] == 0)//enqueue left
		{
			rowY.push(currentY);
			colX.push(currentX-1);
		}
		if(currentY > 0 && gbCopy[currentY-1][currentX] == 0)//enqueue up
		{
			rowY.push(currentY-1);
			colX.push(currentX);
		}
		if(currentX < COLUMNS - 1 && gbCopy[currentY][currentX+1] == 0)//enqueue right
		{
			rowY.push(currentY);
			colX.push(currentX+1);
		}
	}
	return perimeter; //vector of all uncovered tiles to use for parsing to list later
}


// coveredTiles includes surrounding tiles marked as flag or is covered ('X')
int countCoveredTiles(int row, int col, char (&pb)[ROWS][COLUMNS]) {
	int total = 0;

	for (int rowOff = -1; rowOff <= 1; ++rowOff) {
		for (int colOff = -1; colOff <= 1; ++colOff) {
			if (rowOff != 0 || colOff != 0) {
				int r = row + rowOff;
				int c = col + colOff;

				if (r > -1 && r < ROWS && c > -1 && c < COLUMNS) {
					if (pb[r][c] == 'X' || pb[r][c] == 'f')
						++total;
				}
			}
		}
	}

	return total;
}

// flags on tiles that is 100% chance of being the mine, i.e corner tiles or when number
// of surrounding tiles = the value of the current tile (the number of nearby mines)
bool aiEasyFlagger(char (&pb)[ROWS][COLUMNS], list<coord*>& numberedTiles, int& numFlags)
{
	bool success = false;

	for (auto current_tile : numberedTiles) {
		int current_r = current_tile->row;
		int current_c = current_tile->col;
		int mineCountOfCurrentTile = current_tile->value; 

		if (mineCountOfCurrentTile == countCoveredTiles(current_r, current_c, pb)) {
			//look at the surrounding 8 tiles
			for (int rowOff = -1; rowOff <= 1; ++rowOff) {
				for (int colOff = -1; colOff <= 1; ++colOff) {
					if (rowOff != 0 || colOff != 0) {
						int row = current_r + rowOff;
						int col = current_c + colOff;

						if (row > -1 && row < ROWS && col > -1 && col < COLUMNS) {
							if (pb[row][col] == 'X') {
								pb[row][col] = 'f';
								std::cout << "Flagged tile at: " << row << "," << col << endl;
								++numFlags;

								if (success != true)
									success = true;

								printArray(pb, numFlags);
							}
						}
					}
				}
			}
		}
	}

	return success;
}


//	clicks on surrounding tiles of tiles that are "finished" --- finished tiles are tiles which have the number of flagged tiles surrounding it equal to its tile value
//	after clicking on a tile, BFS to uncover more tiles, either empty tiles (0) or more numbered tiles (indicating nearby mines)
//	adds the new numbered tiles to the Tiles_to_Add vector aka temp vector in playGameAI function
bool aiEasyDig(char (&pb)[ROWS][COLUMNS], list<coord*>& finishedTiles, vector<coord>& Tiles_to_Add, int gbCopy[ROWS][COLUMNS], int& numFlags)
{
	bool success = false;

	for (auto tile : finishedTiles) {
		int current_r = tile->row;
		int current_c = tile->col;
		int mineCountOfCurrentTile = tile->value; 

		//look at the surrounding 8 tiles
		for (int rowOff = -1; rowOff <= 1; ++rowOff) {
			for (int colOff = -1; colOff <= 1; ++colOff) {
				if (rowOff != 0 || colOff != 0) {
					int row = current_r + rowOff;
					int col = current_c + colOff;

					if (row > -1 && row < ROWS && col > -1 && col < COLUMNS) {
						if (pb[row][col] == 'X') {
							pb[row][col] = '0' + gbCopy[row][col];
							std::cout << "Clicked tile at: " << row << "," << col << endl;

							if (success != true)
								success = true;

							vector<coord> temp = uncoverZeroBFS(pb, gbCopy, row, col);

							if (!temp.empty()) {
								for (auto t : temp) {
									Tiles_to_Add.push_back(t);
								}
							}	
			
							printArray(pb, numFlags);
						}
					}
				}
			}
		}
	}

	auto iter = finishedTiles.begin();

	while (iter != finishedTiles.end()) {
		coord* temp = *iter;
		iter = finishedTiles.erase(iter);
		delete temp;
	}

	return success;
}

// a tile is "done" when the number of surrounding flagged tiles match its tile number
bool done_(coord* tile, char (&pb)[ROWS][COLUMNS]) {
	int total = 0;
	int value = tile->value;
	int current_r = tile->row;
	int current_c = tile->col;

	for (int rowOff = -1; rowOff <= 1; ++rowOff) {
		for (int colOff = -1; colOff <= 1; ++colOff) {
			if (rowOff != 0 || colOff != 0) {
				int row = current_r + rowOff;
				int col = current_c + colOff;

				if (row > -1 && row < ROWS && col > -1 && col < COLUMNS) {
					if (pb[row][col] == 'f') {
						++total;
					}
				}
			}
		}
	}

	if (total == value) {
		return true;
	}
	else
		return false;
}

void checkTileList(list<coord*>& numberedTiles, char (&pb)[ROWS][COLUMNS], list<coord*>& finishedTiles) {
	auto iter = numberedTiles.begin();

	while (iter != numberedTiles.end()) {
		if (done_(*iter, pb)) {
			finishedTiles.push_back(*iter);
			iter = numberedTiles.erase(iter);
		}
		else 
			++iter;
	}
}

int checkWinCondition(char (&pb)[ROWS][COLUMNS], int gbCopy[ROWS][COLUMNS]) {
	int flagCount = 0;

	for (int row = 0; row < ROWS; ++row) {
		for (int col = 0; col < COLUMNS; ++col) {
			if (pb[row][col] == 'f' && gbCopy[row][col] == 9) {
				++flagCount;
			}
		}
	}

	if (flagCount == minesPlaced)
		return 1;
	else 
		return 0;
}

void clickedMine(char (&pb)[ROWS][COLUMNS], int row, int col, int& numFlags) {
	cout << "AI has clicked on a bomb at tile: " << row << "," << col << endl;
	pb[row][col] = 'B';
	printArray(pb, numFlags);
	cout << "-------------------> Game over" << endl;
	exit (0);
}

void updateProcessingTileList(char (&pb)[ROWS][COLUMNS], int gbCopy[ROWS][COLUMNS], vector<coord>& Tiles_to_Add, int row, int col) {
	vector<coord> temp = uncoverZeroBFS(pb, gbCopy, row, col);

	if (!temp.empty()) {
		for (auto t : temp) {
			Tiles_to_Add.push_back(t);
		}
	}
}

//empty tiles are numbered tiles or tiles with '-'
/*
int countEmptyTilesAround(char (&pbCopy)[ROWS][COLUMNS], int row, int col) {
	int count = 0;

	bool openUp = false, openDown = false, openLeft = false, openRight = false;
	if (row == 0) openUp = true;
	if (col == 0) openLeft = true;
	if (row == ROWS-1) openDown = true;
	if (col == COLUMNS-1) openRight = true;

	if (!openUp && (pbCopy[row-1][col] != 'X' || pbCopy[row-1][col] != 'f')) ++count;
	if (!openLeft && (pbCopy[row][col-1] != 'X' || pbCopy[row][col-1] != 'f')) ++count; 
	if (!openDown && (pbCopy[row+1][col] != 'X' || pbCopy[row+1][col] != 'f')) ++count; 
	if (!openRight && (pbCopy[row][col+1] != 'X' || pbCopy[row][col+1] != 'f')) ++count; 
	if (!openUp && !openLeft && (pbCopy[row-1][col-1] != 'X' || pbCopy[row-1][col-1] != 'f')) ++count;
	if (!openUp && !openRight && (pbCopy[row-1][col+1] != 'X' || pbCopy[row-1][col+1] != 'f')) ++count;
	if (!openDown && !openLeft && (pbCopy[row+1][col-1] != 'X' || pbCopy[row+1][col-1] != 'f')) ++count;
	if (!openDown && !openRight && (pbCopy[row+1][col+1] != 'X' || pbCopy[row+1][col+1] != 'f')) ++count;

	return count;
}*/

int countFlagsAround(bool (&array)[ROWS][COLUMNS], int row, int col) {
	int count = 0;

	bool openUp = false, openDown = false, openLeft = false, openRight = false;
	if (row == 0) openUp = true;
	if (col == 0) openLeft = true;
	if (row == ROWS-1) openDown = true;
	if (col == COLUMNS-1) openRight = true;

	if (!openUp && array[row-1][col]) ++count;
	if (!openLeft && array[row][col-1]) ++count; 
	if (!openDown && array[row+1][col]) ++count; 
	if (!openRight && array[row][col+1]) ++count; 
	if (!openUp && !openLeft && array[row-1][col-1]) ++count;
	if (!openUp && !openRight && array[row-1][col+1]) ++count;
	if (!openDown && !openLeft && array[row+1][col-1]) ++count;
	if (!openDown && !openRight && array[row+1][col+1]) ++count;

	return count;
}


bool customListFind(list<pair<int,int>>& tileSet, pair<int,int>& p) {
	int r = p.first;
	int c = p.second;

	for (pair<int,int>& tile : tileSet) {
		if (tile.first == r && tile.second == c)
			return true;
	}

	return false;
}

bool customVectorFind(vector<pair<int,int>>& tileSet, pair<int,int>& p) {
	int r = p.first;
	int c = p.second;

	for (pair<int,int>& tile : tileSet) {
		if (tile.first == r && tile.second == c)
			return true;
	}

	return false;
}

vector<vector<pair<int,int>>> groupSections(vector<pair<int,int>> borderTiles, char (&pb)[ROWS][COLUMNS]) {
	vector<vector<pair<int,int>>> connectedBorderTiles;
	vector<pair<int,int>> processedTiles;

	while (true) {
		list<pair<int,int>> queue;
		vector<pair<int,int>> finishedSection;

		for (pair<int,int>& p: borderTiles) {
			if (!customVectorFind(processedTiles, p)) {
				queue.push_back(p);
				break;
			}
		}

		if (queue.empty())
			break;

		while (!queue.empty()) {
			pair<int,int> currentTile = queue.front();
			queue.pop_front();
			int current_row = currentTile.first;
			int current_col = currentTile.second;

			finishedSection.push_back(currentTile);
			processedTiles.push_back(currentTile);

			//Find all connecting tiles

			for (pair<int,int>& tile : borderTiles) {
				int tile_row = tile.first;
				int tile_col = tile.second;

				bool isConnected = false;

				if (customVectorFind(finishedSection, tile))
					continue;

				if (abs(current_row - tile_row) <= 2 || abs(current_col - tile_col) <= 2) {
					for (int r = 0; r < ROWS; ++r) {
						for (int c = 0; c < COLUMNS; ++c) {
							if (pb[r][c] != 'X' && pb[r][c] != 'f') {
								if (abs(current_col - c) <= 1 &&
									abs(current_row - r) <= 1 &&
									abs(tile_col - c) <= 1 &&
									abs(tile_row - r) <= 1) {
									
									isConnected = true;
									break;
								}
							}
						}
						if (isConnected)
							break;
					}
				}

				if (!isConnected)
					continue;

				if (!customListFind(queue, tile))
					queue.push_back(tile);
			}
		}

		connectedBorderTiles.push_back(finishedSection);
	}

	return connectedBorderTiles;
}

bool isBoundry(char (&pb)[ROWS][COLUMNS], int row, int col) {
	bool openUp, openDown, openRight, openLeft;

	if (pb[row][col] == 'X')
		return false;

	if (row == 0) openUp = true;
	if (col == 0) openLeft = true;
	if (row == COLUMNS-1) openDown = true;
	if (col == ROWS-1) openRight = true;

	if (!openUp && pb[row-1][col] == '-') return true;
	if(!openUp && pb[row-1][col] == '-') return true;
    if(!openLeft && pb[row][col-1] == '-') return true;
    if(!openDown && pb[row+1][col] == '-') return true;
    if(!openRight && pb[row][col+1] == '-') return true;
    if(!openUp && !openLeft && pb[row-1][col-1] == '-') return true;
    if(!openUp && !openRight && pb[row-1][col+1] == '-') return true;
    if(!openDown && !openLeft && pb[row+1][col-1] == '-') return true;
    if(!openDown && !openRight && pb[row+1][col+1] == '-') return true;

	return false;
}

vector<pair<int,int>> getBorderTiles(char (&pb)[ROWS][COLUMNS]) {
	vector<pair<int,int>> borderTiles;

	for (int row = 0; row < ROWS; ++row) {
		for (int col = 0; col < COLUMNS; ++col) {
			if (isBoundry(pb, row, col) && pb[row][col] != 'f')
				borderTiles.push_back(pair<int,int> (row,col));
		}
	}

	return borderTiles;
}

vector<pair<int,int>> getCoveredTiles(char (&pb)[ROWS][COLUMNS]) {
	vector<pair<int,int>> coveredTiles;

	for (int row = 0; row < ROWS; ++row) {
		for (int col = 0; col < COLUMNS; ++col) {
			if (pb[row][col] == 'X')
				coveredTiles.push_back(pair<int,int> (row,col));
		}
	}

	return coveredTiles;
}

int countFlagsAroundDEBUG (bool (&array)[ROWS][COLUMNS], int row, int col) {
	int count = 0;

	bool openUp = false, openDown = false, openLeft = false, openRight = false;
	if (row == 0) openUp = true;
	if (col == 0) openLeft = true;
	if (row == ROWS-1) openDown = true;
	if (col == COLUMNS-1) openRight = true;

	if (!openUp && array[row-1][col]) 
		cout << row-1 << "," << col << " bool value: " << array[row-1][col] << endl;
	if (!openLeft && array[row][col-1]) ++count; 
		cout << row << "," << col-1 << " bool value: " << array[row][col-1] << endl;
	if (!openDown && array[row+1][col]) ++count; 
		cout << row+1 << "," << col << " bool value: " << array[row+1][col] << endl;
	if (!openRight && array[row][col+1]) ++count; 
		cout << row << "," << col+1 << " bool value: " << array[row][col+1] << endl;
	if (!openUp && !openLeft && array[row-1][col-1]) 
		cout << row-1 << "," << col-1 << " bool value: " << array[row-1][col-1] << endl;
	if (!openUp && !openRight && array[row-1][col+1]) 
		cout << row-1 << "," << col+1 << " bool value: " << array[row-1][col+1] << endl;
	if (!openDown && !openLeft && array[row+1][col-1]) 
		cout << row+1 << "," << col-1 << " bool value: " << array[row+1][col-1] << endl;
	if (!openDown && !openRight && array[row+1][col+1])
		cout << row+1 << "," << col+1 << " bool value: " << array[row+1][col+1] << endl;

	return 0;
}

//modified from "https://luckytoilet.wordpress.com/2012/12/23/2125/"
void tankRecursion(vector<pair<int,int>>& borderTiles, int k, char (&pbCopy)[ROWS][COLUMNS], bool borderOptimization, vector<vector<bool>>& section_solutions, bool (&knownMines)[ROWS][COLUMNS], bool (&knownEmpty)[ROWS][COLUMNS]) {
	int flagCount = 0;
	for (int row = 0; row < ROWS; ++row) {
		for (int col = 0; col < COLUMNS; ++col) {
			//count flags for endgame case

			if (knownMines[row][col] == true)
				++flagCount;

			if (pbCopy[row][col] == '-' || pbCopy[row][col] == 'f' || pbCopy[row][col] == 'X')
				continue;
			
			int tileValue = pbCopy[row][col] - '0';
			int surround = 0;

			if ((row ==0 && col == 0) || (row == ROWS-1 && col==COLUMNS-1))
				surround = 3;
			else if (row == 0 || col == 0 || row == ROWS-1 || col == COLUMNS-1) 
				surround = 5;
			else 
				surround = 8;

			int numFlags = countFlagsAround(knownMines, row, col);
			int numFreeTiles = countFlagsAround(knownEmpty, row, col);

			//case 1: too many mines
			if (numFlags > tileValue) return;

			//cout <<row << "," << col << endl;
			//cout << surround << " " << numFreeTiles << " " << tileValue << endl;
			//case 2: too many numbered/empty tiles
			if (surround - numFreeTiles < tileValue) {
				//countFlagsAroundDEBUG(knownEmpty, row, col);
				//printArray(pbCopy, 0);
				return;
			}
		}
	}

	// too many flags
	if (flagCount > minesPlaced) return;

	if (k == borderTiles.size()) {
		if (!borderOptimization && flagCount < minesPlaced)
			return;
		
		vector<bool> solution;

		for (int i = 0; i < borderTiles.size(); ++i) {
			pair<int,int> p = borderTiles.at(i);
			int sr = p.first;
			int sc = p.second;
			solution.push_back(knownMines[sr][sc]);
		}

		section_solutions.push_back(solution);
		return;
	}

	pair<int,int> q = borderTiles.at(k);
	int qr = q.first;
	int qc = q.second;

	//recurse two points: mine and no mine
	knownMines[qr][qc] = true;
	tankRecursion(borderTiles,k+1, pbCopy, borderOptimization, section_solutions, knownMines, knownEmpty);
	knownMines[qr][qc] = false;

	knownEmpty[qr][qc] = true;
	tankRecursion(borderTiles,k+1, pbCopy, borderOptimization, section_solutions, knownMines, knownEmpty);
	knownEmpty[qr][qc] = false;
}


//modified from "https://luckytoilet.wordpress.com/2012/12/23/2125/"
bool tankSolver(char (&pb)[ROWS][COLUMNS], int gbCopy[ROWS][COLUMNS], int& numFlags, vector<coord>& temp) {
	int bruteRecurseCalls = 0;
	vector<pair<int,int>> borderTiles = getBorderTiles(pb);
	vector<pair<int,int>> coveredTiles = getCoveredTiles(pb);

	bool borderOptimization = false;

	int numberOfSquareOutsideRange = coveredTiles.size() - borderTiles.size();
	if (numberOfSquareOutsideRange > 5) {
		borderOptimization = true;
	}
	else 
		borderTiles = coveredTiles;

	vector<vector<pair<int,int>>> sections;
	if (!borderOptimization) {
		sections.push_back(borderTiles);
	}
	else
		sections = groupSections(borderTiles,pb);

	bool success = false;
	double prob_best = 0;
	int prob_bestTile = -1;
	int prob_best_s = -1;

	for (int s = 0; s < sections.size(); ++s) {
		//create copy of player board
		char pbCopy[ROWS][COLUMNS];

		for(int i = 0; i < ROWS; i++) {
			for(int j = 0; j < COLUMNS; j++) {
				pbCopy[i][j] = pb[i][j];
			}
		}

		vector<vector<bool>> section_solutions;
		bool knownMines[ROWS][COLUMNS];
		bool knownEmpty[ROWS][COLUMNS];

		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLUMNS; ++j) {
				if (pb[i][j] == 'f')
					knownMines[i][j] = true;
				else
					knownMines[i][j] = false;
				
					
				if (pb[i][j] == '-' || (pb[i][j] > '0' && pb[i][j] < '9'))
					knownEmpty[i][j] = true;
				else 
					knownEmpty[i][j] = false;
			}
		}

		tankRecursion(sections.at(s), 0, pbCopy, borderOptimization, section_solutions, knownMines, knownEmpty);

		//something went wrong
		if (section_solutions.size() == 0) {
			return false;
		}

		//check for solved tiles

		for (int i = 0; i < sections.at(s).size(); ++i) {
			bool allMine = true;
			bool allEmpty = true;
			for (vector<bool>& sol : section_solutions) {
				if (!sol.at(i)) allMine = false;
				if (sol.at(i)) allEmpty = false; 
			}

			pair<int,int> p = sections.at(s).at(i);
			int pr = p.first;
			int pc = p.second;

			if (allMine) {
			//	success = true;
				std::cout << "Tank Solver -> Flagged tile at: " << pr << "," << pc << endl;
				pb[pr][pc] = 'f';
				++numFlags;
				printArray(pb, numFlags);
			}
			
			if (allEmpty) {
				success = true;
				if (pb[pr][pc] == 'X') {
					std::cout << "Tank Solver -> Clicked on tile: " << pr << "," << pc << endl;
					pb[pr][pc] = '0' + gbCopy[pr][pc];
					if (gbCopy[pr][pc] == 0)
						updateProcessingTileList(pb, gbCopy, temp, pr, pc);

					printArray(pb, numFlags);
				}
			}
		}

		//calculate possibilities
		if (success) continue;
		int maxEmpty = -10000;
		int iEmpty = -1;
		for (int i = 0; i < sections.at(s).size(); ++i) {
			int nEmpty = 0;
			for (vector<bool>& sol : section_solutions) {
				if (!sol[i]) ++nEmpty;
			}
			if (nEmpty > maxEmpty) {
				maxEmpty = nEmpty;
				iEmpty = i;
			}
		}

		double probability = (double) maxEmpty/ (double)section_solutions.size();

		if (probability > prob_best) {
			prob_best = probability;
			prob_bestTile = iEmpty;
			prob_best_s = s;
		}
	}

	if (success)
		return true;

	//guessing time
	pair<int,int> g = sections.at(prob_best_s).at(prob_bestTile);
	int gr = g.first;
	int gc = g.second;

		
	if (pb[gr][gc] == 'X') {
		std::cout << "Tank Solver -> MAKING GUESS -> Clicked on tile: " << gr << "," << gc << endl;

		if (gbCopy[gr][gc] == 9)
			clickedMine(pb, gr, gc, numFlags);

		
		pb[gr][gc] = '0' + gbCopy[gr][gc];
		if (gbCopy[gr][gc] == 0)
			updateProcessingTileList(pb, gbCopy, temp, gr, gc);

		printArray(pb, numFlags);
	}
	else {
		while (true) {
			// more guessing
			int r = rand() % ROWS;
			int c = rand() % COLUMNS;

			if (pb[r][c] == 'X') {
				std::cout << "Tank Solver -> MAKING *RANDOM* GUESS -> Clicked on tile: " << r << "," << c << "\n";

				if (gbCopy[r][c] == 9)
					clickedMine(pb, r, c, numFlags);

				pb[r][c] = '0' + gbCopy[r][c];

				if (gbCopy[gr][gc] == 0)
					updateProcessingTileList(pb, gbCopy, temp, r, c);

				printArray(pb, numFlags);
				break;
			}
		}
	}
	

	return true;
}
		
//*******************************************************************************************************
//*******************************************************************************************************
void playGameAI(int gb[ROWS][COLUMNS])
{
	list<coord*> numberedTiles;
	list<coord*> finishedTiles;
	int numFlags = 0;
	vector<coord> temp; //to store tiles found in bfs
	
	//initialize new game board
	char pb[ROWS][COLUMNS];
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			pb[i][j] = 'X';
		}
	}
	
	bool firstMoveSuccess = false;

	while (!firstMoveSuccess) {
		int startingRow = rand() % ROWS;
		int startingCol = rand() % COLUMNS;

		if (gb[startingRow][startingCol] == 9) {
			//starting tile is a mine, restart
			continue;
		}
		else if (gb[startingRow][startingCol] == 0) {
			//starting tile is an empty tile, start BFS to reveal surrounding tiles
			std::cout << "Random click at tile: " << startingRow << "," << startingCol << endl;
			temp = uncoverZeroBFS(pb, gb, startingRow, startingCol);
			firstMoveSuccess = true;
		}
		else {
			//starting tile is a numbered tile, need to make random clicks until an empty tile is clicked
			std::cout << "Random click at tile: " << startingRow << "," << startingCol << endl;
			pb[startingRow][startingCol] = '0' + gb[startingRow][startingCol];
			printArray(pb,numFlags);

			while (true) {
				int row = rand() % ROWS;
				int col = rand() % COLUMNS;

				if (pb[row][col] == 'X') {
					if (gb[row][col] == 9) {
						//mine is clicked by guessing... AI lost
						pb[row][col] = 'M';
						printArray(pb, numFlags);
						std::cout << "AI has clicked on a mine at tile: " << row << "," << col << endl;
						std::cout << "-------------------> Game over" << endl;
						return;
					}
					else if (gb[row][col] == 0) {
						std::cout << "Random click at tile: " << row << "," << col << endl;
						temp = uncoverZeroBFS(pb, gb, row, col);
						break;
					}
					else {
						//another numbered tile is clicked... continue to make random clicks
						std::cout << "Random click at tile: " << row << "," << col << endl;
						pb[row][col] = '0' + gb[row][col];
						printArray(pb,numFlags);
						continue;
					}
				}	
			}

			firstMoveSuccess = true;
		}
	}

	printArray(pb, numFlags);
	

	//iteration is temporary used to stop the while loop after n number of times for testing purposes

	int counter = 0;

	while(true) 
	{	
		//add to list (when zero was picked and BFS happened)
		for(int k = 0; k < temp.size(); k++)
		{
			coord* tempNode = new coord(temp[k].value ,temp[k].row, temp[k].col);
			numberedTiles.push_back(tempNode);
		}

		temp.clear();

		bool successFlag = aiEasyFlagger(pb, numberedTiles, numFlags);
		checkTileList(numberedTiles, pb, finishedTiles);
		bool successDig = aiEasyDig(pb, finishedTiles, temp, gb, numFlags);

		if (checkWinCondition(pb,gb)) {
			std::cout << "All of the mines has been flagged!. AI Wins." << endl;
			return;
		}

		if (!successFlag && !successDig) {
			tankSolver(pb, gb, numFlags, temp);
			++counter;

			if (checkWinCondition(pb,gb)) {
				std::cout << "All of the mines has been flagged!. AI Wins." << endl;
				return;
			}

			if (counter > 1) {
				//more guessing

				while (true) {
					int row = rand() % ROWS;
					int col = rand() % COLUMNS;

					if (pb[row][col] == 'X') {
						if (gb[row][col] == 9) {
							clickedMine(pb, row, col, numFlags);
						}
						else if (gb[row][col] == 0) {
							std::cout << "Random click at tile: " << row << "," << col << endl;
							temp = uncoverZeroBFS(pb, gb, row, col);
							break;
						}
						else {
							//another numbered tile is clicked... continue to make random clicks
							std::cout << "Random click at tile: " << row << "," << col << endl;
							printArray(pb,numFlags);
						}	
					}
				}
				counter = 0;
			}
		}

		if (checkWinCondition(pb,gb)) {
			std::cout << "All of the mines has been flagged!. AI Wins." << endl;
			return;
		}
	}
}

void playGameUser(int gb[ROWS][COLUMNS])
{
	char pb[ROWS][COLUMNS];
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			pb[i][j] = 'X';
		}
	}
	char choice;
	bool goodPick = true;
	int x = 99;
	int y = 99;
	int numFlags = 0;
	//bool gameFinished = false;
	
	while(true)
	{
		if (goodPick)
			printArray(pb, NUMBOMBS - numFlags);
		while(true)
		{	
			if(!goodPick)
				break;
			std::cout << "Please Enter 'd' for dig, 'f' for flag, or 'u' for unflag (d / f / u): ";
			std::cin >> choice;
			if(choice == 'd' || choice == 'D' || choice == 'f' || choice == 'F' || choice == 'u' || choice == 'U')
			{
				if((choice == 'u' || choice == 'U') && numFlags == 0)
					std::cout << "there are no flags. please choose another choice\n";
				else
					break;
			}
			else
				std::cout << "\ninvalid choice\n";
		}
		while(true)
		{
			std::cout << "\nEnter your X coordinate: ";
			std::cin >> x;
			std::cout << "\nEnter your Y coordinate: ";
			std::cin >> y;
			goodPick = true;
 
			if(x >= 0 && x < COLUMNS && y >= 0 && y < ROWS)
			{
				break;
			}
			else
			{
				std::cout << "\nEither X or Y coordinate is out of bounds, Please enter a valid y,x coordinate" << endl << endl;
				goodPick = false;
			}
		}
		switch(choice)
		{
			case 'd': case 'D':
			{
				if(pb[y][x] != 'X')
				{
					std::cout << "Tile is already Exposed, Please enter a new coordinates.\n";
					goodPick = false;
					break;
				}
				if(gb[y][x] == 9)
				{
					std::cout << "KABOOM!! YOU LOSE";
					pb[y][x] = '*';
					exit(1);
				}
				if(gb[y][x] == 0)
				{
					vector<coord> dontCare = uncoverZeroBFS(pb, gb, y, x); //show all adj zeros and their adjacent numbers
					goodPick = true;
				}
				else
					pb[y][x] = '0' + gb[y][x]; // show number
					goodPick = true;
				break;
			}
			case 'f': case 'F':
			{
				if(pb[y][x] == 'X')
				{
					pb[y][x] = 'f';
					numFlags++;
					goodPick = true;
					break;
				}
				if(pb[y][x] == 'f')
				{
					std::cout << "This tile is already flagged, Please pick another." << endl;
					goodPick = false;
					break;
				}
				else
				{
					std::cout << "This tile is already exposed, Please pick another." << endl;
					goodPick = false;
					break;
				}
			}
			case 'u': case 'U':
			{
				if(pb[y][x] == 'f')
				{
					pb[y][x] = 'X';
					numFlags--;
					goodPick = true;
					break;
				}
				else
				{
					std::cout << "That tile doesn't have a flag, Please enter a new coordinate.";
					goodPick = false;
					break;
				}
			}
		}
		if (goodPick)
		{
			bool gameFinished = false;
			int picks = 0;
			
			for(int p = 0; p < ROWS; p++)
			{
				for(int s = 0; s < COLUMNS; s++)
				{
					if(pb[p][s] != 'X' && pb[p][s] != 'f')
						picks+=1;
				}
			}
			if(picks == NOBOMBS)
			{
				printArray(pb, NUMBOMBS - numFlags);
				std::cout << "CONGRATULATIONS! YOU WON!" << endl;
				exit(1);
			}
		}
	}
}
#endif


	
















