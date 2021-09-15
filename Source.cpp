#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <windows.h>

using plane = std::vector<std::vector<int>>;

bool checkWin(plane board)
{
	for (auto i : board)
		for (auto j : i)
			if (j == 2048)
				return true;
	return false;
}

bool checkLoss(plane board)
{
	for (int i = 0; i < board.size(); i++) 
		for (int j = 0; j < board[i].size(); j++) 
			if (!board[i][j] || (i && board[i][j] == board[i - 1][j]) || (j && board[i][j] == board[i][j - 1])) return false;
	return true;
}

bool move(int &score, char dir, plane& board)
{
	bool moved = false;
	std::vector<std::vector<bool>> newNums = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	switch (dir) {
	case('l'):
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board[i].size(); j++) {
				if (board[i][j] && j > 0) {
					if (!board[i][j - 1]) {
						board[i][j - 1] = board[i][j];
						board[i][j] = 0;
						moved = true;
						j -= 2;
					} else if (board[i][j - 1] == board[i][j] && !newNums[i][j - 1] && !newNums[i][j]) {
						board[i][j - 1] *= 2;
						score += board[i][j] * 2;
						board[i][j] = 0;
						newNums[i][j - 1] = true;
						moved = true;
						j -= 2;
					}
				}
			}
		}
		break;


	case('u'):
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board[i].size(); j++) {
				if (board[i][j] && i > 0) {
					if (!board[i - 1][j]) {
						board[i - 1][j] = board[i][j];
						board[i][j] = 0;
						moved = true;
						i -= 1;
						j -= 1;
					}
					else if (board[i - 1][j] == board[i][j] && !newNums[i - 1][j] && !newNums[i][j]) {
						board[i - 1][j] *= 2;
						score += board[i][j] * 2;
						board[i][j] = 0;
						newNums[i - 1][j] = true;
						moved = true;
						i -= 1;
						j -= 1;
					}
				}
			}
		}
		break;


	case('r'):
		for (int i = board.size() - 1; i >= 0; i--) {
			for (int j = board[i].size() - 1; j >= 0; j--) {
				if (board[i][j] && j < board[i].size() - 1) {
					if (!board[i][j + 1]) {
						board[i][j + 1] = board[i][j];
						board[i][j] = 0;
						moved = true;
						j += 2;
					} else if (board[i][j + 1] == board[i][j] && !newNums[i][j + 1] && !newNums[i][j]) {
						board[i][j + 1] *= 2;
						score += board[i][j] * 2;
						board[i][j] = 0;
						newNums[i][j + 1] = true;
						moved = true;
						j += 2;
					}
				}
			}
		}
		break;


	case('d'):
		for (int i = board.size() - 1; i >= 0; i--) {
			for (int j = board[i].size() - 1; j >= 0; j--) {
				if (board[i][j] && i < board.size() - 1) {
					if (!board[i + 1][j]) {
						board[i + 1][j] = board[i][j];
						board[i][j] = 0;
						moved = true;
						i += 1;
						j += 1;
					} else if (board[i + 1][j] == board[i][j] && !newNums[i + 1][j] && !newNums[i][j]) {
						board[i + 1][j] *= 2;
						score += board[i][j] * 2;
						board[i][j] = 0;
						newNums[i + 1][j] = true;
						moved = true;
						i += 1;
						j += 1;
					}
				}
			}
		}
		break;
	}
	return moved;
}

plane rotate(plane toRotate)
{
	plane rotated = toRotate;
	for (int i = 0; i < toRotate.size(); i++)
		for (int j = 0; j < toRotate[i].size(); j++)
			rotated[j][i] = toRotate[i][j];
	return rotated;
}

void displayScreen(int score, plane board, int newest)
{
	std::string curr;
	std::vector<int> bigs;
	int currBiggest;
	for (auto i : rotate(board)) {
		currBiggest = std::to_string(i[0]).size();
		for (auto j : i)
			if (std::to_string(j).size() > currBiggest) 
				currBiggest = std::to_string(j).size();
		bigs.push_back(currBiggest);
	}

	plane rotated = rotate(board);
	std::cout << "Score: " << score << "\n";
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (i == newest / 1000 && j == newest % 1000)
				curr = "(" + std::to_string(board[i][j]) + ")";
			else
				curr = "[" + (board[i][j] ? std::to_string(board[i][j]) : " ") + "]";
			std::cout << curr;
			for (int i = curr.size(); i < bigs[j] + 3; i++)
				std::cout << ' ';
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

/*
void EspawnTile(plane& board)
{
	std::vector<int*> empties;
	for (int i = 0; i < board.size(); i++)
		for (int j = 0; j < board[i].size(); j++)
			if (!board[i][j]) empties.push_back(&board[i][j]);

	if (empties.size())
		*empties[rand() % empties.size()] = (rand() % 10 ? 2 : 4);
}
*/
int spawnTile(plane &board)
{
	std::vector<int> empties;
	for (int i = 0; i < board.size(); i++) 
		for (int j = 0; j < board[i].size(); j++) 
			if (!board[i][j]) empties.push_back(i * 1000 + j);

	if (empties.size()) {
		int pos = empties[rand() % empties.size()];
		board[pos / 1000][pos % 1000] = (rand() % 10 ? 2 : 4);
		return pos;
	}
	return -1001;
}

void gameLoop()
{
	plane board = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	int score = 0;
	bool l = false, u = false, r = false, d = false, won = false;
	std::string inp;
	spawnTile(board);
	spawnTile(board);
	displayScreen(score, board, -1001);
	while (true) {

		if (GetAsyncKeyState(37) && !l) {
			l = true;
			if (move(score, 'l', board)) {
				displayScreen(score, board, spawnTile(board));
				if (!won && checkWin(board)) {
					won = true;
					std::cout << "You won! Continue? ";
					std::cin >> inp;
					if (inp[0] != 'Y' && inp[0] != 'y') {
						std::cout << "\n";
						return;
					}
				} 
				if (checkLoss(board)) {
					std::cout << "you lost! ";
					return;
				}
			}
		} else if (!GetAsyncKeyState(37) && l) l = false;


		if (GetAsyncKeyState(38) && !u) {
			u = true;
			if (move(score, 'u', board)) {
				displayScreen(score, board, spawnTile(board));
				if (!won && checkWin(board)) {
					won = true;
					std::cout << "You won! Continue? ";
					std::cin >> inp;
					if (inp[0] != 'Y' && inp[0] != 'y') {
						std::cout << "\n";
						return;
					}
				}
				if (checkLoss(board)) {
					std::cout << "you lost! ";
					return;
				}
			}
		} else if (!GetAsyncKeyState(38) && u) u = false;


		if (GetAsyncKeyState(39) && !r) {
			r = true;
			if (move(score, 'r', board)) {
				displayScreen(score, board, spawnTile(board));
				if (!won && checkWin(board)) {
					won = true;
					std::cout << "You won! Continue? ";
					std::cin >> inp;
					if (inp[0] != 'Y' && inp[0] != 'y') {
						std::cout << "\n";
						return;
					}
				}
				if (checkLoss(board)) {
					std::cout << "you lost! ";
					return;
				}
			}
		} else if (!GetAsyncKeyState(39) && r) r = false;


		if (GetAsyncKeyState(40) && !d) {
			d = true;
			if (move(score, 'd', board)) {
				displayScreen(score, board, spawnTile(board));
				if (!won && checkWin(board)) {
					won = true;
					std::cout << "You won! Continue? ";
					std::cin >> inp;
					if (inp[0] != 'Y' && inp[0] != 'y') {
						std::cout << "\n";
						return;
					}
				}
				if (checkLoss(board)) {
					std::cout << "you lost! ";
					return;
				}
			}
		} else if (!GetAsyncKeyState(40) && d) d = false;
	}
}

int main()
{
	std::string inp;
	srand(time(NULL));
	do {
		gameLoop();
		std::cout << "Restart? ";
		std::cin >> inp;
	} while (inp[0] == 'Y' || inp[0] == 'y');
	return 0;
	/*
	for (int i = 0; i < 256; i++) {
		std::cout << "if (GetAsyncKeyState(" << i << ")) {\n	std::cout << " << i << " << \"\\n\"; \n}\n";
	}*/
}