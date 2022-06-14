#include <string>
#include <iostream>
using namespace std;
int prev_row_code = 0;
extern int code_input_index;
extern int rotateCounter;
extern int direction;
extern int axis;
extern void rotate(int direction, int indexsOfRotation(int, int), int offset);
extern int indexesOfRotationX(int j, int offset);
extern int indexesOfRotationY(int j, int offset);
extern int indexesOfRotationZ(int j, int offset);
extern int N;
extern string code_s;

void makeMoves(string code_s) {
	int row = 0;
	if (code_input_index != -1 && code_input_index < code_s.length() && rotateCounter < 0) {
		while (code_s[code_input_index] >= '0' && code_s[code_input_index] <= '9') {
			row *= 10;
			row += code_s[code_input_index] - '0';
			code_input_index++;
		}
		if (row)
			row--;
		//cerr << row << "|" << code_s[code_input_index] << endl;

		if (code_s[code_input_index] == '^') {
			row = prev_row_code;
			code_s[code_input_index] = code_s[code_input_index - 1];
		}

		if (code_s[code_input_index] == 'M' || code_s[code_input_index] == 'm' || code_s[code_input_index] == 'E' || code_s[code_input_index] == 'e' || code_s[code_input_index] == 'S' || code_s[code_input_index] == 's')
			row = 1;
		direction = 0;
		switch (code_s[code_input_index]) {
		case 'S':
		case 'F':
			direction = 1;
		case 's':
		case 'f':
			axis = axis = 2 + row * 3;
			rotate(direction, indexesOfRotationZ, row);
			break;
		case 'r': //fix -> zamiana 'R' z 'r'
			direction = 1;
		case 'R':
			axis = 0 + row * 3;
			rotate(direction, indexesOfRotationX, row);
			break;
		case 'e':
		case 'd':
			direction = 1;
		case 'E':
		case 'D':
			axis = 1 + row * 3;
			rotate(direction, indexesOfRotationY, row);
			break;
		case 'b':
			direction = 1;
		case 'B':
			axis = 2 + (N - 1) * 3 - row * 3;
			rotate(direction, indexesOfRotationZ, N - 1 - row);
			break;
		case 'M':
		case 'L':
			direction = 1;
		case 'm':
		case 'l':
			axis = 0 + (N - 1) * 3 - row * 3;
			rotate(direction, indexesOfRotationX, N - 1 - row);
			break;
		case 'U':
			direction = 1;
		case 'u':
			axis = 1 + (N - 1) * 3 - row * 3;
			rotate(direction, indexesOfRotationY, N - 1 - row);
			break;
		}
		code_input_index++;
		prev_row_code = row;
		if (code_input_index == code_s.length())
			code_input_index = -1;
	}
}



extern bool solved;
string generateScramble(int length, string frontText = "") {
	solved = false;
	string* everyMove = new string[6 * N];
	string baseMoves = "FfRrDdBbLlUu";
	int i = 0;
	string output = "", curr = "";
	int prev = -1;
	if (N%2) //nieparzyste
		for (i = 0; i < length; i++) {
			curr = "";
			if (rand() % 2) {
				curr = "";
				int temp = rand() % ((N - 1) / 2);
				if (temp)
					curr += to_string(temp + 1);
				int letter = rand() % 6;
				if (letter % 2)
					if (prev == letter - 1) {
						i--;
					}
					else {
						output += curr;
						output += baseMoves[letter];
						prev = letter;
					}
				else if (prev == letter + 1) {
					i--;
				}
				else {
					output += curr;
					output += baseMoves[letter];
					prev = letter;
				}	
			}
			else {
				curr = "";
				int temp = rand() % ((N - 1) / 2);
				if (temp)
					curr += to_string(temp + 1);
				int letter = rand() % 6 + 6;
				if (letter % 2)
					if (prev == letter - 1) {
						i--;
					}
					else {
						output += curr;
						output += baseMoves[letter];
						prev = letter;
					}
				else if (prev == letter + 1) {
					i--;
				}
				else {
					output += curr;
					output += baseMoves[letter];
					prev = letter;
				}
			}
		}
	else //parzyste
		for (int i = 0; i < length; i++) {
			curr = "";
			int temp = rand() % (N / 2);
			if (temp)
				curr += to_string(temp+1);
			int letter = rand() % 12;
			if (letter % 2)
				if (prev == letter - 1) {
					i--;
				}
				else {
					output += curr;
					output += baseMoves[letter];
					prev = letter;
				}
			else if (prev == letter + 1) {
				i--;
			}
			else {
				output += curr;
				output += baseMoves[letter];
				prev = letter;
			}
		}

	cerr << endl << frontText << output << endl;

	code_s = output;
	code_input_index = 0;

	delete[] everyMove;
	return output;
}