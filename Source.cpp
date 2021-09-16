#include <iostream> // I/O
#include <vector> // Can't return arrays or take them as arguments
#include <random> // For spawning new tiles
#include <ctime> // For a random seed
#include <string> // Need `std::to_string()` to make the output lines even
#include <windows.h> // For movement


	// Board size, feel free to change
const int sizeX = 6, sizeY = 9;


using plane = std::vector<std::vector<int>>;

bool checkWin(plane board)
{
		// Quite self explanatory
	for (auto i : board)
		for (auto j : i)
			if (j == 2048)
				return true;
	return false;
}

bool checkLoss(plane board)
{
		// Checking if any tiles are zero or if any pairs of adjacent tiles are equal (can be combined)
		// otherwise, there are no legal moves
	for (int i = 0; i < board.size(); i++) 
		for (int j = 0; j < board[i].size(); j++) 
			if (!board[i][j] 
				|| (i && board[i][j] == board[i - 1][j]) 
				|| (j && board[i][j] == board[i][j - 1])) 
					return false;
	return true;
}


bool move(int &score, char dir, plane& board)
{
	bool moved = false; // Return this to see if any movement was made, to avoid skipping turns
	std::vector<std::vector<bool>> newNums(sizeY, std::vector<bool>(sizeX)); // A tile combined in a move cannot combine again that same move

	switch (dir) {
	case('l'):
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board[i].size(); j++) {
				if (board[i][j] && j > 0) {
						// Movement
					if (!board[i][j - 1]) {
						board[i][j - 1] = board[i][j];
						board[i][j] = 0;
						moved = true;
						j -= 2;
					} 
						// Combination of tiles
					else if (board[i][j - 1] == board[i][j] && !newNums[i][j - 1] && !newNums[i][j]) {
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
					} 
					
					else if (board[i][j + 1] == board[i][j] && !newNums[i][j + 1] && !newNums[i][j]) {
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
					} 
					
					else if (board[i + 1][j] == board[i][j] && !newNums[i + 1][j] && !newNums[i][j]) {
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

int spawnTile(plane &board)
{
		// Store the position of all empty cells
	std::vector<int> empties;
	for (int i = 0; i < board.size(); i++) 
		for (int j = 0; j < board[i].size(); j++) 
			if (!board[i][j]) empties.push_back(i * board.size() + j); 

		// Choose one of those cells at random and add either a 2 or a 4 there (randomly, again)
	if (empties.size()) {
		int pos = empties[rand() % empties.size()];
		board[pos / board.size()][pos % board.size()] = (rand() % 10 ? 2 : 4);
		return pos;
	}
		// This will be interpreted as [-1][-1], which appears nowhere (because no tile was added)
	return 0 - board.size() - 1;
}

plane rotate(plane toRotate)
{
		// Needed for displayScreen() where I check the length of the numbers by coloumn and not by row
	plane rotated(sizeX, std::vector<int>(sizeY));
	for (int i = 0; i < toRotate.size(); i++)
		for (int j = 0; j < toRotate[i].size(); j++)
			rotated[j][i] = toRotate[i][j];
	return rotated;
}
void displayScreen(int score, plane board, int newest)
{
	std::string curr;
	std::vector<int> bigs;

		// Get the longest number of each coloumn, so that I can have the entire coloumn the same length
	for (auto i : rotate(board)) {
		int currBiggest = std::to_string(i[0]).size();
		for (auto j : i)
			if (std::to_string(j).size() > currBiggest) 
				currBiggest = std::to_string(j).size();
		bigs.push_back(currBiggest);
	}

	std::cout << "Score: " << score << "\n";
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			if (i == newest / board.size() && j == newest % board.size())
				curr = "(" + std::to_string(board[i][j]) + ")";
			else
				curr = "[" + (board[i][j] ? std::to_string(board[i][j]) : " ") + "]";

				// Space coloumns appart equally on all layers by adding a padding of ' 's to the shorter numbers
			std::cout << curr;
			for (int i = curr.size(); i < bigs[j] + 3; i++) 
				std::cout << ' ';
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}


bool turn(plane& board, int& score, bool& won, char dir)
{
		// Handle one turn of the game

	std::string inp;

	if (move(score, dir, board)) {

		displayScreen(score, board, spawnTile(board));

		if (!won && checkWin(board)) {
			won = true; // Make sure you can only win once per game

			std::cout << "You won! Continue? ";
			std::cin >> inp;

			if (inp[0] != 'Y' && inp[0] != 'y') {
				std::cout << "\n";
				return true;
			}
		}

			// Not using an else if because it is literally possible to win and lose in a single move
		if (checkLoss(board)) { 
			std::cout << "you lost! ";
			return true;
		}
	}

		// This function will return true if gameLoop should be returned from as well
	return false;
}

void gameLoop()
{
	plane board(sizeY, std::vector<int>(sizeX));
	int score = 0;
		// I'm using these bools to make sure the movement is only counted when the button is initially pressed
		// Except won which is used to prevent the player from winning numerous times in a single game
	bool l = false, u = false, r = false, d = false, won = false;
		
	
		// Each game starts with two tiles
	spawnTile(board);
	spawnTile(board);
	displayScreen(score, board, -1001);
	
	while (true) {

		if (GetAsyncKeyState(37) && !l) {
			l = true;
			if (turn(board, score, won, 'l'))
				return;
		} else if (!GetAsyncKeyState(37) && l) l = false;

		if (GetAsyncKeyState(38) && !u) {
			u = true;
			if (turn(board, score, won, 'u'))
				return;
		} else if (!GetAsyncKeyState(38) && u) u = false;

		if (GetAsyncKeyState(39) && !r) {
			r = true;
			if (turn(board, score, won, 'r'))
				return;
		} else if (!GetAsyncKeyState(39) && r) r = false;

		if (GetAsyncKeyState(40) && !d) {
			d = true;
			if (turn(board, score, won, 'd'))
				return;
		} else if (!GetAsyncKeyState(40) && d) d = false;
	}
}


int main()
{
		// Pretty trivial main() function, but of course, it is necessary
	std::string inp;
	srand(time(NULL));
	do {
		gameLoop();
		std::cout << "Restart? ";
		std::cin >> inp;
	} while (inp[0] == 'Y' || inp[0] == 'y');
	return 0;
}
