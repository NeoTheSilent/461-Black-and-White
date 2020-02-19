#include <iostream>
#include <random>
#include <math.h>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

// Black or White : A Variation by Raymond Rennock

// More will be explained in the accomponied doc, but I've included a lot of my
// thoughts in the comments of the program. Of note, I recieved a lot of assistance
// during this project from other classmates, including Tyler Wheaton as I frequently got 
// lost/confused during this project (i.e: Using bad methods to solve it, memory overload) 
//and he offered assistance to me in explaining me to the correct lines of thought and 
// algorithms. While his work and mine do have similarities as a result of our shared 
// thought processes, I did my best to make this project my own, not just a clone. 

// This is a special note to make it clear I'm not trying to hide the help I recieved.
// Without them, this code would simply only produce an algorithm that would let you play
// Black or White, rather than showing the computer solving it. Regardless, I'd like to think
// that I made this project my own with the help of my friends.

const int gamesize = 5;	

struct State {
	//The grid itself
	unsigned int Grid;

	//A function that will randomly scatter B and W across the board.
	//Note: This will only work so long as <random> is loaded in
	void randomize() {
		//We're using a random device to randomize the board
		//Boring, stale, but it gets the job done
		random_device random;

		//We're using binary to store the game state. It's more data efficient
		//This way, every possible state is between 0 and 33554431 (for a total of 33,554,432 possible states)
		// Unfortunately, it has to be manually changed if we want a bigger board since I couldn't
		// find an easy way to make this changeable (I tried (sum(2,(sum(gamesize,2)))) but
		// it's a double, and that doesn't mix well with unsigned int)
		Grid = (random() % 33554431);
	}


	//This is mainly how the manual version of the game functions.
	unsigned int FlipPiece(unsigned int state, int row, int column) {
		//We create a temp state to return as our answer later without overriding
		//what was input
		unsigned int tempState = 0;	
		//Since we're using binary, it's important to have a remainder
		bool remain;

		//Now we flip over all correct positions (i.e: All X row, all Y column)
		for (int i = 0; i < gamesize * gamesize; i++) {
			remain = state % 2;
			if (!abs(i / gamesize - row) || !abs(i % gamesize - column)) {
				tempState += remain ? 0	: (int)pow(2, i);
			}
			else {
				tempState += remain ? (int)pow(2, i) : 0;
			}
			state /= 2;
		}
		
		return tempState;
	}

	// This is used to display what the current board state is.
	void display(unsigned int state) {
		int newline = 1;
		for (int i = 0; i < pow(gamesize,2); i++) {
			if (state % 2 == 0) {
				cout << "[ ] ";
			}
			else {
				cout << "[W] ";
			}
			if (newline == gamesize) {
				cout << endl;
				newline = 1;
			}
			else {
				newline += 1;
			}
			state /= 2;
		}
	}
};

struct Solved {
	int row;
	int col;
	unsigned next;

	Solved() {
		row = 0;
		col = 0;
		next = 0;
	}

	Solved(int row, int col, unsigned next) {
		this->row = row;
		this->col = col;
		this->next = next;
	}
};

void expand(queue<unsigned int>& q, unordered_map<unsigned int, Solved>& Solve){
	unsigned int current, next = q.front();
	State temp;
	q.pop();

	for (int i = 0; i < pow(gamesize, 2); i++) {
		current = temp.FlipPiece(next, i / gamesize, i % gamesize);
		if (Solve.count(current) == 0) {
			temp.display(current);
			cout << current << endl; 
			Solve.emplace(current, Solved(i / gamesize, i %	gamesize, next));
			q.push(current);
		}
	}
}

void solveMe(unsigned int start, unordered_map<unsigned int, Solved>& solution) {
	//Start is successfully being put in. However, it's reading in as the default.
	Solved record = solution[start];
	State temp;

	while (record.next != -1) {
		//system("CLS");
		cout << "Current State: " << endl;
		temp.display(record.next);
		record = solution[record.next];
		cout << endl;
		system("pause");
	}
};
	
int main() {
	// We're setting up our map and queue so we can solve this automatically
	// if we wish. Note: Because we're not using any storage area, it'll take quite a
	// while to generate every time
	unordered_map<unsigned int, Solved> solution;
	queue<unsigned int> q;
	
	// The warning:
	cout << "Welcome to Black or White: Please know that it will take several minutes to generate solutions, so please be patient!" << endl;

	solution.emplace(0, Solved(-1, -1, -1));
	q.push(0);

	while (!q.empty()) {
		expand(q, solution);
	}
	system("CLS");
	cout << solution.size() << " different game states have been found" << endl;

	//This is our gamestate. At this point, we're randomizing it
	//and then displaying it.
	State X;
	X.randomize();
	cout << endl << "Randomization Complete: This is the starting board state: " << endl;
	X.display(X.Grid);

	//After, the player gets the choice if they want to solve it or if they want to give up.
	char choice;
	cout << "Would you like see the solution instead of solving it? (Y/N): ";
	cin >> choice;



	string quit = "no";
	while (quit == "no") {
		if ((choice == 'Y') || (choice == 'y')) {
			//... This is if you wish to manually solve this.
			solveMe(X.Grid, solution);
			cout << endl << "We have reached a goal state. Thank you for using the program." << endl;
			quit = "yes";
			system("pause");
		}

		// If the player wishes to manually solve it, then the computer's solution doesn't
		// really matter. This is purely on them solving it now
		else if ((choice == 'N') || (choice == 'n')) {

			//We're initializing our variables that are relevant here:
			//Row is for the row that will be changed
			//Column is for the column that will be changed
			//Moves is for counting how many moves we've had.
			int row = -1;
			int moves = 0;
			int column = 0;

			//It's a bit crude, but we're asking the player to enter 0 at the start of 
			//every turn in order to quit. Also, since we're also asking for a row, we're
			//killing 2 birds with 1 stone by making it a check if they want to quit (0)
			//or if they want to solve (1-5). Any other number will make them select again.

			//Note: If you enter an invalid option (i.e: "Hey, I wanna press Q") it'll just
			//break. Also, selecting more than 1 number will also cause it to be unhappy.
			//As of right now, I haven't fixed this since it's not the highest of priorities.
			while (row != 0)
			{
				cout << "Please enter a row (1-5) or enter 0 to quit: ";
				cin >> row;

				//If they picked a valid row
				if ((0 < row) && (row < 6)) {
					//We're putting them in another loop to make sure that everything works
					//as intended, and they only leave if they want to leave normally, or
					//if they pick a valid target
					string check = "y";
					cout << "Please enter a column (1-5) or enter 0 to cancel input: ";
					cin >> column;

					while (check == "y") {
						//If they picked a valid column
						if ((0 < column) && (column < 6)) {
							//We flip it, and any appropriate rows/columns 
							//Note: Row and Column are intentionally -1 since the user
							//will likely assume that the top left most space is [1,1]
							//instead of [0,0]. That, and 0 is meant for quiting.
							X.Grid = X.FlipPiece(X.Grid, row - 1, column - 1);
							//Afterwords, we clear the screen, increase the moves counter
							//(as they've made a move) and display what they did.
							system("CLS");
							moves += 1;
							cout << "Move: " << moves << "You have flipped row " << row
								<< " and column" << column << "." << endl;
							X.display(X.Grid);
							//We then break the check.
							check = "n";
						}
						//If they picked an invalid target, we make them chose again
						//(Since this will put them back at the start of the loop, which
						//asks for another input)
						else if (column != 0) {
							cout << endl;
						}
						//If they want to cancel the entire input, we let them (as they
						//might have put in a row they want to change)
						else if (column == 0) {
							check = "n";
							quit = "yes";
							cout << "Input Cancelled." << endl << endl;
						}
					}
				}
				//If they picked an invalid target, we make them chose again
				//Since this is the end of the loop, we don't need to do a thing. I'm simply
				//adding a newline to make it look nicer.
				else if (row != 0) {
					cout <<  endl;
				}
				//If they want to quit, we let them. Good riddance.
				else if (row == 0) {
					quit = "yes";
					cout << "Thank you for using this program. Press any button to quit" << endl;
					system("pause");
				}
				//If you solve the puzzle, neat, you get to quit.
				if ((X.Grid == 0) || (X.Grid == 33554431)) {
					system("CLS");
					cout << "You have solved the puzzle in " << moves << " moves.";
					X.display(X.Grid);
					quit = "yes";
					system("pause");
				}
			}
		}

		//If you picked a bad input, we're gonna ask you to try again.
		else {
			cout << "Please choose a valid input: ";
			cin >> choice;
		}
	}
	return 0;
}