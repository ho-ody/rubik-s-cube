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
	if (code_input_index < code_s.length() && rotateCounter < 0) {
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

		direction = 0;
		switch (code_s[code_input_index]) {
		case 'F':
			direction = 1;
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
		case 'd':
			direction = 1;
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
		case 'L':
			direction = 1;
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
	}
}




string generateScramble(int length) {
	string* everyMove = new string[6 * N];
	string baseMoves = "FfRrDdBbLlUu";
	
	string output = "", curr = "", prev = "";
	if (N%2) //nieparzyste
		for (int i = 0; i < 12; i++) {



		}
	else //parzyste
		for (int i = 0; i < length; i++) {
			curr = "";
			int temp = rand() % (N / 2);
			if (temp)
				curr += to_string(temp+1);
			curr += baseMoves[rand() % 12];
			if (curr != prev) {
				output += curr;
				prev = curr;
			}
			else {
				i--;
			}
		}

	cerr << output << endl;


	code_s = output;
	code_input_index = 0;

	delete[] everyMove;
	return output;
}