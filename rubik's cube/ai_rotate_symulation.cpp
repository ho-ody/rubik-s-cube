#include <string>
using namespace std;
extern int** rotateIndex; //ustawiane raz, na pocz¹tku dzia³ania programu na podstawie wielkoœci koœci
extern int* ai_toRotate;
extern int N;
extern int*** sides;
void ai_orderUpdateRotateMatrix(bool direction, int axis)
{
	/*
	if ((direction == 0 && axis % 3 != 0) || (direction == 1 && axis % 3 == 0)) {
		for (int x = 0; x < N / 2; x++) {
			for (int y = x; y < N - x - 1; y++) {

				int temp = ai_order[ai_toRotate[x + N * y]];
				ai_order[ai_toRotate[x + N * y]] = ai_order[ai_toRotate[y + (N - 1 - x) * N]];
				ai_order[ai_toRotate[y + (N - 1 - x) * N]] = ai_order[ai_toRotate[(N - 1 - x) + (N - 1 - y) * N]];
				ai_order[ai_toRotate[(N - 1 - x) + (N - 1 - y) * N]] = ai_order[ai_toRotate[(N - 1 - y) + x * N]];
				ai_order[ai_toRotate[(N - 1 - y) + x * N]] = temp;
			}
		}
	}
	else {
		for (int x = 0; x < N / 2; x++) {
			for (int y = x; y < N - x - 1; y++) {

				int temp = ai_order[ai_toRotate[x + N * y]];
				ai_order[ai_toRotate[x + N * y]] = ai_order[ai_toRotate[(N - 1 - y) + x * N]];
				ai_order[ai_toRotate[(N - 1 - y) + x * N]] = ai_order[ai_toRotate[(N - 1 - x) + (N - 1 - y) * N]];
				ai_order[ai_toRotate[(N - 1 - x) + (N - 1 - y) * N]] = ai_order[ai_toRotate[y + (N - 1 - x) * N]];
				ai_order[ai_toRotate[y + (N - 1 - x) * N]] = temp;
			}
		}
	}
	*/
}

extern void showSides();

void worker_side_rotate(int side, bool direction) {
	if (direction) {
		int temp = sides[side][0][0];
		sides[side][0][0] = sides[side][0][2];
		sides[side][0][2] = sides[side][2][2];
		sides[side][2][2] = sides[side][2][0];
		sides[side][2][0] = temp;

		temp = sides[side][0][1];
		sides[side][0][1] = sides[side][1][2];
		sides[side][1][2] = sides[side][2][1];
		sides[side][2][1] = sides[side][1][0];
		sides[side][1][0] = temp;
	}
	else {
		int temp = sides[side][0][0];
		sides[side][0][0] = sides[side][2][0];
		sides[side][2][0] = sides[side][2][2];
		sides[side][2][2] = sides[side][0][2];
		sides[side][0][2] = temp;

		temp = sides[side][0][1];
		sides[side][0][1] = sides[side][1][0];
		sides[side][1][0] = sides[side][2][1];
		sides[side][2][1] = sides[side][1][2];
		sides[side][1][2] = temp;
	}
}

enum sides_code { down = 0, up = 1, right = 2, left = 3, front = 4, back = 5, d = 0, u = 1, r = 2, l = 3, f = 4, b = 5 };
void worker_toside_rotate(int side, bool direction, int offset_i1(int), int offset_j1(int), int offset_i2(int), int offset_j2(int)) {
	int rules[6][4] = { {f,l,b,r},{f,r,b,l},{f,d,b,u},{f,u,b,d},{u,l,d,r},{u,r,d,l} };
	if (direction) {
		int temp[3];
		temp[0] = sides[rules[side][0]][offset_i1(0)][offset_j1(0)];
		temp[1] = sides[rules[side][0]][offset_i1(1)][offset_j1(1)];
		temp[2] = sides[rules[side][0]][offset_i1(2)][offset_j1(2)];
		
		sides[rules[side][0]][offset_i1(0)][offset_j1(0)] = sides[rules[side][1]][offset_i2(0)][offset_j2(0)];
		sides[rules[side][0]][offset_i1(1)][offset_j1(1)] = sides[rules[side][1]][offset_i2(1)][offset_j2(1)];
		sides[rules[side][0]][offset_i1(2)][offset_j1(2)] = sides[rules[side][1]][offset_i2(2)][offset_j2(2)];

		sides[rules[side][1]][offset_i2(0)][offset_j2(0)] = sides[rules[side][2]][offset_i1(0)][offset_j1(0)];
		sides[rules[side][1]][offset_i2(1)][offset_j2(1)] = sides[rules[side][2]][offset_i1(1)][offset_j1(1)];
		sides[rules[side][1]][offset_i2(2)][offset_j2(2)] = sides[rules[side][2]][offset_i1(2)][offset_j1(2)];

		sides[rules[side][2]][offset_i1(0)][offset_j1(0)] = sides[rules[side][3]][offset_i2(0)][offset_j2(0)];
		sides[rules[side][2]][offset_i1(1)][offset_j1(1)] = sides[rules[side][3]][offset_i2(1)][offset_j2(1)];
		sides[rules[side][2]][offset_i1(2)][offset_j1(2)] = sides[rules[side][3]][offset_i2(2)][offset_j2(2)];

		sides[rules[side][3]][offset_i2(0)][offset_j2(0)] = temp[0];
		sides[rules[side][3]][offset_i2(1)][offset_j2(1)] = temp[1];
		sides[rules[side][3]][offset_i2(2)][offset_j2(2)] = temp[2];
	}
	else {

	}


}

void rotate_u(int side, bool direction, int offset) {
	if (offset != 1)
		worker_side_rotate(side, direction);
	int rules[6][4] = { {f,l,b,r},{f,r,b,l},{f,d,b,u},{f,u,b,d},{u,l,d,r},{u,r,d,l} };
	if (direction) {
		int temp[3];
		temp[0] = sides[rules[side][0]][offset][0];
		temp[1] = sides[rules[side][0]][offset][1];
		temp[2] = sides[rules[side][0]][offset][2];

		sides[rules[side][0]][offset][0] = sides[rules[side][1]][offset][0];
		sides[rules[side][0]][offset][1] = sides[rules[side][1]][offset][1];
		sides[rules[side][0]][offset][2] = sides[rules[side][1]][offset][2];

		sides[rules[side][1]][offset][0] = sides[rules[side][2]][offset][0];
		sides[rules[side][1]][offset][1] = sides[rules[side][2]][offset][1];
		sides[rules[side][1]][offset][2] = sides[rules[side][2]][offset][2];

		sides[rules[side][2]][offset][0] = sides[rules[side][3]][offset][0];
		sides[rules[side][2]][offset][1] = sides[rules[side][3]][offset][1];
		sides[rules[side][2]][offset][2] = sides[rules[side][3]][offset][2];

		sides[rules[side][3]][offset][0] = temp[0];
		sides[rules[side][3]][offset][1] = temp[1];
		sides[rules[side][3]][offset][2] = temp[2];
	}
	else {
		int temp[3];
		temp[0] = sides[rules[side][0]][offset][0];
		temp[1] = sides[rules[side][0]][offset][1];
		temp[2] = sides[rules[side][0]][offset][2];

		sides[rules[side][0]][offset][0] = sides[rules[side][3]][offset][0];
		sides[rules[side][0]][offset][1] = sides[rules[side][3]][offset][1];
		sides[rules[side][0]][offset][2] = sides[rules[side][3]][offset][2];

		sides[rules[side][3]][offset][0] = sides[rules[side][2]][offset][0];
		sides[rules[side][3]][offset][1] = sides[rules[side][2]][offset][1];
		sides[rules[side][3]][offset][2] = sides[rules[side][2]][offset][2];

		sides[rules[side][2]][offset][0] = sides[rules[side][1]][offset][0];
		sides[rules[side][2]][offset][1] = sides[rules[side][1]][offset][1];
		sides[rules[side][2]][offset][2] = sides[rules[side][1]][offset][2];

		sides[rules[side][1]][offset][0] = temp[0];
		sides[rules[side][1]][offset][1] = temp[1];
		sides[rules[side][1]][offset][2] = temp[2];
	}
}
void rotate_r(int side, bool direction, int offset) {
	if (offset != 1)
		worker_side_rotate(side, direction);
	int rules[6][4] = { {f,l,b,r},{f,r,b,l},{f,d,b,u},{f,u,b,d},{u,l,d,r},{u,r,d,l} };
	if (direction) {
		int temp[3];
		temp[0] = sides[rules[side][0]][0][2 - offset];
		temp[1] = sides[rules[side][0]][1][2 - offset];
		temp[2] = sides[rules[side][0]][2][2 - offset];

		sides[rules[side][0]][0][2 - offset] = sides[rules[side][1]][0][2 - offset];
		sides[rules[side][0]][1][2 - offset] = sides[rules[side][1]][1][2 - offset];
		sides[rules[side][0]][2][2 - offset] = sides[rules[side][1]][2][2 - offset];

		sides[rules[side][1]][0][2 - offset] = sides[rules[side][2]][2][offset];
		sides[rules[side][1]][1][2 - offset] = sides[rules[side][2]][1][offset];
		sides[rules[side][1]][2][2 - offset] = sides[rules[side][2]][0][offset];

		sides[rules[side][2]][0][offset] = sides[rules[side][3]][2][2 - offset];
		sides[rules[side][2]][1][offset] = sides[rules[side][3]][1][2 - offset];
		sides[rules[side][2]][2][offset] = sides[rules[side][3]][0][2 - offset];

		sides[rules[side][3]][0][2 - offset] = temp[0];
		sides[rules[side][3]][1][2 - offset] = temp[1];
		sides[rules[side][3]][2][2 - offset] = temp[2];
	}
	else {
		int temp[3];
		temp[0] = sides[rules[side][0]][0][2 - offset];
		temp[1] = sides[rules[side][0]][1][2 - offset];
		temp[2] = sides[rules[side][0]][2][2 - offset];

		sides[rules[side][0]][0][2 - offset] = sides[rules[side][3]][0][2 - offset];
		sides[rules[side][0]][1][2 - offset] = sides[rules[side][3]][1][2 - offset];
		sides[rules[side][0]][2][2 - offset] = sides[rules[side][3]][2][2 - offset];

		sides[rules[side][3]][0][2 - offset] = sides[rules[side][2]][2][offset];
		sides[rules[side][3]][1][2 - offset] = sides[rules[side][2]][1][offset];
		sides[rules[side][3]][2][2 - offset] = sides[rules[side][2]][0][offset];

		sides[rules[side][2]][0][offset] = sides[rules[side][1]][2][2 - offset];
		sides[rules[side][2]][1][offset] = sides[rules[side][1]][1][2 - offset];
		sides[rules[side][2]][2][offset] = sides[rules[side][1]][0][2 - offset];

		sides[rules[side][1]][0][2 - offset] = temp[0];
		sides[rules[side][1]][1][2 - offset] = temp[1];
		sides[rules[side][1]][2][2 - offset] = temp[2];
	}
}
void rotate_f(int side, bool direction, int offset) {
	if (offset != 1)
		worker_side_rotate(side, direction);
	int rules[6][4] = { {f,l,b,r},{f,r,b,l},{f,d,b,u},{f,u,b,d},{u,l,d,r},{u,r,d,l} };
	if (direction) {
		int temp[3];
		temp[0] = sides[rules[side][0]][2 - offset][0];
		temp[1] = sides[rules[side][0]][2 - offset][1];
		temp[2] = sides[rules[side][0]][2 - offset][2];

		sides[rules[side][0]][2 - offset][0] = sides[rules[side][1]][0][offset];
		sides[rules[side][0]][2 - offset][1] = sides[rules[side][1]][1][offset];
		sides[rules[side][0]][2 - offset][2] = sides[rules[side][1]][2][offset];

		sides[rules[side][1]][0][offset] = sides[rules[side][2]][offset][2];
		sides[rules[side][1]][1][offset] = sides[rules[side][2]][offset][1];
		sides[rules[side][1]][2][offset] = sides[rules[side][2]][offset][0];

		sides[rules[side][2]][offset][0] = sides[rules[side][3]][0][2 - offset];
		sides[rules[side][2]][offset][1] = sides[rules[side][3]][1][2 - offset];
		sides[rules[side][2]][offset][2] = sides[rules[side][3]][2][2 - offset];

		sides[rules[side][3]][0][2 - offset] = temp[2];
		sides[rules[side][3]][1][2 - offset] = temp[1];
		sides[rules[side][3]][2][2 - offset] = temp[0];
	}
	else {
		int temp[3];
		temp[0] = sides[rules[side][0]][2 - offset][0];
		temp[1] = sides[rules[side][0]][2 - offset][1];
		temp[2] = sides[rules[side][0]][2 - offset][2];

		sides[rules[side][0]][2 - offset][0] = sides[rules[side][3]][2][2 - offset];
		sides[rules[side][0]][2 - offset][1] = sides[rules[side][3]][1][2 - offset];
		sides[rules[side][0]][2 - offset][2] = sides[rules[side][3]][0][2 - offset];

		sides[rules[side][3]][0][2 - offset] = sides[rules[side][2]][offset][0];
		sides[rules[side][3]][1][2 - offset] = sides[rules[side][2]][offset][1];
		sides[rules[side][3]][2][2 - offset] = sides[rules[side][2]][offset][2];

		sides[rules[side][2]][offset][0] = sides[rules[side][1]][2][offset];
		sides[rules[side][2]][offset][1] = sides[rules[side][1]][1][offset];
		sides[rules[side][2]][offset][2] = sides[rules[side][1]][0][offset];

		sides[rules[side][1]][0][offset] = temp[0];
		sides[rules[side][1]][1][offset] = temp[1];
		sides[rules[side][1]][2][offset] = temp[2];
	}
}
void rotate_b(int side, bool direction, int offset) {
	if (offset != 1)
		worker_side_rotate(side, direction);
	int rules[6][4] = { {f,l,b,r},{f,r,b,l},{f,d,b,u},{f,u,b,d},{u,l,d,r},{u,r,d,l} };
	if (direction) {
		int temp[3];
		temp[0] = sides[rules[side][0]][offset][0];
		temp[1] = sides[rules[side][0]][offset][1];
		temp[2] = sides[rules[side][0]][offset][2];

		sides[rules[side][0]][offset][0] = sides[rules[side][1]][2][offset];
		sides[rules[side][0]][offset][1] = sides[rules[side][1]][1][offset];
		sides[rules[side][0]][offset][2] = sides[rules[side][1]][0][offset];

		sides[rules[side][1]][0][offset] = sides[rules[side][2]][2 - offset][0];
		sides[rules[side][1]][1][offset] = sides[rules[side][2]][2 - offset][1];
		sides[rules[side][1]][2][offset] = sides[rules[side][2]][2 - offset][2];

		sides[rules[side][2]][2 - offset][0] = sides[rules[side][3]][2][2 - offset];
		sides[rules[side][2]][2 - offset][1] = sides[rules[side][3]][1][2 - offset];
		sides[rules[side][2]][2 - offset][2] = sides[rules[side][3]][0][2 - offset];

		sides[rules[side][3]][0][2 - offset] = temp[0];
		sides[rules[side][3]][1][2 - offset] = temp[1];
		sides[rules[side][3]][2][2 - offset] = temp[2];
	}
	else {
		int temp[3];
		temp[0] = sides[rules[side][0]][offset][2];
		temp[1] = sides[rules[side][0]][offset][1];
		temp[2] = sides[rules[side][0]][offset][0];

		sides[rules[side][0]][offset][0] = sides[rules[side][3]][0][2 - offset];
		sides[rules[side][0]][offset][1] = sides[rules[side][3]][1][2 - offset];
		sides[rules[side][0]][offset][2] = sides[rules[side][3]][2][2 - offset];

		sides[rules[side][3]][0][2 - offset] = sides[rules[side][2]][2 - offset][2];
		sides[rules[side][3]][1][2 - offset] = sides[rules[side][2]][2 - offset][1];
		sides[rules[side][3]][2][2 - offset] = sides[rules[side][2]][2 - offset][0];

		sides[rules[side][2]][2 - offset][0] = sides[rules[side][1]][0][offset];
		sides[rules[side][2]][2 - offset][1] = sides[rules[side][1]][1][offset];
		sides[rules[side][2]][2 - offset][2] = sides[rules[side][1]][2][offset];

		sides[rules[side][1]][0][offset] = temp[0];
		sides[rules[side][1]][1][offset] = temp[1];
		sides[rules[side][1]][2][offset] = temp[2];
	}
}

void ai_rotate(int direction, int axis) {
	//showSides();
	//u: rotate_u(u, 1, 2);
	//U: rotate_u(u, 0, 2);
	//D: rotate_u(d, 0, 0);
	//d: rotate_u(d, 1, 0);
	//E: rotate_u(d, 0, 1);
	//e: rotate_u(d, 1, 1);
	
	//R: rotate_r(r, 0, 0);
	//r: rotate_r(r, 1, 0);
	//L: rotate_r(l, 0, 2);
	//l: rotate_r(l, 1, 2);
	//M: rotate_r(l, 0, 1);
	//m: rotate_r(l, 1, 1);
	
	//F: rotate_f(f, 0, 2);
	//f: rotate_f(f, 1, 2);
	//B: rotate_b(b, 0, 2);
	//b: rotate_b(b, 1, 2);
	//S: rotate_f(f, 0, 1);
	//s:rotate_f(f, 1, 1);
	//showSides();
}
extern int ai_code_index;
string ai_wholecode = "";
void ai_makeMoves(string* ai_code) {
	string code_s = *ai_code;
	*ai_code = "";
	int axis = -1;
	int row = 0;
	int prev_row_code = 0;
	int direction = -1;
	ai_wholecode += code_s;

	for (int id = 0; id < code_s.length(); id++) {
		direction = 0;
		switch (code_s[id]) {
		case 'u':
			rotate_u(u, 1, 2);
			break;
		case 'U':
			rotate_u(u, 0, 2);
			break;
		case 'd':
			rotate_u(d, 1, 0);
			break;
		case 'D':
			rotate_u(d, 0, 0);
			break;
		case 'e':
			rotate_u(d, 1, 1);
			break;
		case 'E':
			rotate_u(d, 0, 1);
			break;
		case 'r':
			rotate_r(r, 1, 0);
			break;
		case 'R':
			rotate_r(r, 0, 0);
			break;
		case 'l':
			rotate_r(l, 1, 2);
			break;
		case 'L':
			rotate_r(l, 0, 2);
			break;
		case 'm':
			rotate_r(l, 1, 1);
			break;
		case 'M':
			rotate_r(l, 0, 1);
			break;
		case 'f':
			rotate_f(f, 1, 2);
			break;
		case 'F':
			rotate_f(f, 0, 2);
			break;
		case 'b':
			rotate_b(b, 1, 2);
			break;
		case 'B':
			rotate_b(b, 0, 2);
			break;
		case 's':
			rotate_f(f, 1, 1);
			break;
		case 'S':
			rotate_f(f, 0, 1);
			break;
		}
		prev_row_code = row;
	}
}