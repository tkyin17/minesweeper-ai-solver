//Written by Pieter Molenaar, Thien Nguyen, Amir Seifpour

#include "minesweeper.h"
#include <iostream>

using namespace std;

int main()
{
	
	//initialize game board
	int gameBoard[ROWS][COLUMNS] = {0};
	//printArray(gameBoard);
	
	//add bombs (bombs are 9's)
	addMines(gameBoard);
	cout << "Mine placements: \n\n";
	printArray(gameBoard);
	
	//add bomb adjacency numbers
	addNumbers(gameBoard);
	cout << "Numbers added: \n\n";
	printArray(gameBoard);

	//User plays game
	//playGameUser(gameBoard);
	playGameAI(gameBoard);
	return 0;
	
}
	
	
	