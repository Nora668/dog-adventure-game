#include "Board.hpp"
#include <iostream>
using namespace std;

Board::Board(char diff, bool d) {
	level = diff;
	debug = d;
	wallStrength = 6;
	InitAll();
}
Board::Board(bool d) {
	level = 'e';
	debug = d;
	wallStrength = 6;
	InitAll();
}
Board::Board(char diff, string name, bool d) {
	level = diff;
	debug = d;
	mydog.name = name;
	wallStrength = 6;
	InitAll();
}
void Board::InitAll() {
	bool keepPlaying = true;
	while (keepPlaying) {
		cout << "What level of difficulty do you want (e, m, or h)?" << endl;
		char c;
		cin >> c;
		level = c;
		startx = rand() % size;
		starty = 0;
		endx = rand() % size;
		endy = size - 1;
		mydog.x = startx;
		mydog.y = starty;
		boardConfig();
		addFood();
		addTraps();
		printBoard();
		playGame();
		cout << "Play again? " << endl;
		string s = "no";
		cin >> s;
		if (s == "yes" || s == "Yes" || s == "Y" || s == "y") {
			keepPlaying = true;
			mydog.reset();
		} else {
			cout << "Thanks for playing!" << endl;
			keepPlaying = false;
		}
	}
}
void Board::playGame() {
	bool play = true;
	while (play) {
		cout << "Move (u, d, l, r) " << endl;
		char c;
		cin >> c;
		play = moveDog(c);
		printBoard();
	}
}
void Board::addFood() {
	int amt;
	if (level == 'e') {
		amt = size;
		for (int i = 0; i < amt; i++) {
			int randRow = rand() % 18 + 1;
			int randCol = rand() % 18 + 1;
			board[randRow][randCol] = 'F';
		}
	} else if (level == 'm') {
		amt = size - 2;
		for (int i = 0; i < amt; i++) {
			int randRow = rand() % 18 + 1;
			int randCol = rand() % 18 + 1;
			board[randRow][randCol] = 'F';
		}
	} else if (level == 'h') {
		amt = size - 4;
		for (int i = 0; i < amt; i++) {
			int randRow = rand() % 18 + 1;
			int randCol = rand() % 18 + 1;
			board[randRow][randCol] = 'F';
		}
	} else {
		cout << "Invalid entry, try again" << endl;
	}
}
void Board::addTraps() {
	int amt;
	if (level == 'h') {
		amt = size;
		for (int i = 0; i < amt; i++) {
			int randRow = rand() % 18 + 1;
			int randCol = rand() % 18 + 1;
			board[randRow][randCol] = 'T';
		}
	} else if (level == 'm') {
		amt = size - 2;
		for (int i = 0; i < amt; i++) {
			int randRow = rand() % 18 + 1;
			int randCol = rand() % 18 + 1;
			board[randRow][randCol] = 'T';
		}
	} else if (level == 'e') {
		amt = size - 4;
		for (int i = 0; i < amt; i++) {
			int randRow = rand() % 18 + 1;
			int randCol = rand() % 18 + 1;
			board[randRow][randCol] = 'T';
		}
	} else {
		cout << "Invalid entry, try again" << endl;
	}
}
void Board::boardConfig() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = '+';
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == 0 || i == size - 1) {
				board[i][j] = '-';
			} else if (j == 0 || j == size - 1) {
				board[i][j] = '|';
			}
		}
	}
	int maxWalls = 0;
	if (level == 'e') {
		maxWalls = 6;
	} else if (level == 'm') {
		maxWalls = 12;
	} else if (level == 'h') {
		maxWalls = 16;
	}
	for (int i = 1; i < size - 1; i += 2) {
		for (int j = 0; j < maxWalls; j++) {
			int column = rand() % 18 + 1;
			board[i][column] = '-';
		}
	}
	for (int i = 1; i < size - 1; i++) {
		for (int j = 0; j < maxWalls; j++) {
			int row = rand() % 18 + 1;
			board[row][i] = '|';
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == '+') {
				board[i][j] = ' ';
			}
		}
	}
	board[startx][starty] = 'D';
	board[endx][endy] = 'E';
}

void Board::printBoard() {
	mydog.printDog();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << board[i][j];
		}
		cout << endl;
	}
}

bool Board::moveDog(char c) {
	if (c == 'u') {
		if (mydog.y >= 1) {
			if (board[mydog.x - 1][mydog.y] == '|' || board[mydog.x - 1][mydog.y] == '-') {
				string s;
				if (mydog.strength >= 6) {
					cout << "Do you want to knock down the wall?" << endl;
					cin >> s;
					if (s == "Yes" || s == "yes") {
						int amt = -6;
						mydog.changeStrength(amt);
					} else if (s == "No" || s == "no") {
						return false;
					}
				}
			}
			board[mydog.x][mydog.y] = ' ';
			mydog.x--;
			if (board[mydog.x][mydog.y] == ' ') {
				int amt = -2;
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'F') {
				int amt = rand() % 16 + 2;
				mydog.changeStrength(amt);
			} else if (board[mydog.x][mydog.y] == 'T') {
				int amt = -(rand() % 16 + 2);
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'E') {
				mydog.won();
				return false;
			}
			board[mydog.x][mydog.y] = 'D';
			if (mydog.strength > 0) {
				return true;
			} else {
				mydog.die();
				return false;
			}
		}
	}
	else if (c == 'd') {
		if (mydog.y <= size - 1) {
			if (board[mydog.x + 1][mydog.y] == '|' || board[mydog.x + 1][mydog.y] == '-') {
				string s;
				if (mydog.strength >= 6) {
					cout << "Do you want to knock down the wall?" << endl;
					cin >> s;
					if (s == "Yes" || s == "yes") {
						int amt = -6;
						mydog.changeStrength(amt);
					} else if (s == "No" || s == "no") {
						return false;
					}
				}
			}
			board[mydog.x][mydog.y] = ' ';
			mydog.x++;
			if (board[mydog.x][mydog.y] == ' ') {
				int amt = -2;
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'F') {
				int amt = rand() % 16 + 2;
				mydog.changeStrength(amt);
			} else if (board[mydog.x][mydog.y] == 'T') {
				int amt = -(rand() % 16 + 2);
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'E') {
				mydog.won();
				return false;
			}
			board[mydog.x][mydog.y] = 'D';
			if (mydog.strength > 0) {
				return true;
			} else {
				mydog.die();
				return false;
			}
		}
	}
	else if (c == 'l') {
		if (mydog.x < size - 1) {
			if (board[mydog.x][mydog.y - 1] == '|' || board[mydog.x][mydog.y - 1] == '-') {
				string s;
				if (mydog.strength >= 6) {
					cout << "Do you want to knock down the wall?" << endl;
					cin >> s;
					if (s == "Yes" || s == "yes") {
						int amt = -6;
						mydog.changeStrength(amt);
					} else if (s == "No" || s == "no") {
						return false;
					}
				}
			}
			board[mydog.x][mydog.y] = ' ';
			mydog.y--;
			if (board[mydog.x][mydog.y] == ' ') {
				int amt = -2;
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'F') {
				int amt = rand() % 16 + 2;
				mydog.changeStrength(amt);
			} else if (board[mydog.x][mydog.y] == 'T') {
				int amt = -(rand() % 16 + 2);
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'E') {
				mydog.won();
				return false;
			}
			board[mydog.x][mydog.y] = 'D';
			if (mydog.strength > 0) {
				return true;
			} else {
				mydog.die();
				return false;
			}
		}
	}
	else if (c == 'r') {
		if (mydog.x > 1) {
			if (board[mydog.x][mydog.y + 1] == '|' || board[mydog.x][mydog.y + 1] == '-') {
				string s;
				if (mydog.strength >= 6) {
					cout << "Do you want to knock down the wall?" << endl;
					cin >> s;
					if (s == "Yes" || s == "yes") {
						int amt = -6;
						mydog.changeStrength(amt);
					} else if (s == "No" || s == "no") {
						return false;
					}
				}
			}
			board[mydog.x][mydog.y] = ' ';
			mydog.y++;
			if (board[mydog.x][mydog.y] == ' ') {
				int amt = -2;
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'F') {
				int amt = rand() % 16 + 2;
				mydog.changeStrength(amt);
			} else if (board[mydog.x][mydog.y] == 'T') {
				int amt = -(rand() % 16 + 2);
				mydog.changeStrength(amt);
			}
			if (board[mydog.x][mydog.y] == 'E') {
				mydog.won();
			}
			board[mydog.x][mydog.y] = 'D';
			if (mydog.strength > 0) {
				return true;
			} else {
				mydog.die();
				return false;
			}
		}
	} else {
		cout << "Invalid entry, try again" << endl;
	}
	return true;
}
