#include <string>
#include "ai_rotate_symulation.h"
using namespace std;

extern int*** sides;
string ai_wholecode = "";
enum sides_code { down = 0, up = 1, right = 2, left = 3, front = 4, back = 5, d = 0, u = 1, r = 2, l = 3, f = 4, b = 5 };

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

string simplify_final_code() {
	//triple symbols reduce
	for (int i = 0; i < ai_wholecode.length();) {
		if (ai_wholecode[i] == 'u' && ai_wholecode[i + 1] == 'u' && ai_wholecode[i + 2] == 'u') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "U"); i = 0;
		}
		else if (ai_wholecode[i] == 'U' && ai_wholecode[i + 1] == 'U' && ai_wholecode[i + 2] == 'U') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "u"); i = 0;
		}
		else if (ai_wholecode[i] == 'd' && ai_wholecode[i + 1] == 'd' && ai_wholecode[i + 2] == 'd') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "D"); i = 0;
		}
		else if (ai_wholecode[i] == 'D' && ai_wholecode[i + 1] == 'D' && ai_wholecode[i + 2] == 'D') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "d"); i = 0;
		}
		else if (ai_wholecode[i] == 'f' && ai_wholecode[i + 1] == 'f' && ai_wholecode[i + 2] == 'f') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "F"); i = 0;
		}
		else if (ai_wholecode[i] == 'F' && ai_wholecode[i + 1] == 'F' && ai_wholecode[i + 2] == 'F') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "f"); i = 0;
		}
		else if (ai_wholecode[i] == 'r' && ai_wholecode[i + 1] == 'r' && ai_wholecode[i + 2] == 'r') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "R"); i = 0;
		}
		else if (ai_wholecode[i] == 'R' && ai_wholecode[i + 1] == 'R' && ai_wholecode[i + 2] == 'R') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "r"); i = 0;
		}
		else if (ai_wholecode[i] == 'b' && ai_wholecode[i + 1] == 'b' && ai_wholecode[i + 2] == 'b') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "B"); i = 0;
		}
		else if (ai_wholecode[i] == 'B' && ai_wholecode[i + 1] == 'B' && ai_wholecode[i + 2] == 'B') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "b"); i = 0;
		}
		else if (ai_wholecode[i] == 'l' && ai_wholecode[i + 1] == 'l' && ai_wholecode[i + 2] == 'l') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "L"); i = 0;
		}
		else if (ai_wholecode[i] == 'L' && ai_wholecode[i + 1] == 'L' && ai_wholecode[i + 2] == 'L') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "l"); i = 0;
		}
		else if (ai_wholecode[i] == 'm' && ai_wholecode[i + 1] == 'm' && ai_wholecode[i + 2] == 'm') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "M"); i = 0;
		}
		else if (ai_wholecode[i] == 'M' && ai_wholecode[i + 1] == 'M' && ai_wholecode[i + 2] == 'M') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "m"); i = 0;
		}
		else if (ai_wholecode[i] == 'e' && ai_wholecode[i + 1] == 'e' && ai_wholecode[i + 2] == 'e') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "E"); i = 0;
		}
		else if (ai_wholecode[i] == 'E' && ai_wholecode[i + 1] == 'E' && ai_wholecode[i + 2] == 'E') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "e"); i = 0;
		}
		else if (ai_wholecode[i] == 's' && ai_wholecode[i + 1] == 's' && ai_wholecode[i + 2] == 's') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "S"); i = 0;
		}
		else if (ai_wholecode[i] == 'S' && ai_wholecode[i + 1] == 'S' && ai_wholecode[i + 2] == 'S') {
			ai_wholecode.erase(i, 3);
			ai_wholecode.insert(i, "s"); i = 0;
		}
		else
			i++;
	}
	//opposite symbols reduce
	for (int i = 0; i < ai_wholecode.length();) {
		if (ai_wholecode[i] == 'u' && ai_wholecode[i + 1] == 'U') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'U' && ai_wholecode[i + 1] == 'u') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'd' && ai_wholecode[i + 1] == 'D') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'D' && ai_wholecode[i + 1] == 'd') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'f' && ai_wholecode[i + 1] == 'F') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'F' && ai_wholecode[i + 1] == 'f') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'r' && ai_wholecode[i + 1] == 'R') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'R' && ai_wholecode[i + 1] == 'r') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'b' && ai_wholecode[i + 1] == 'B') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'B' && ai_wholecode[i + 1] == 'b') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'l' && ai_wholecode[i + 1] == 'L') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'L' && ai_wholecode[i + 1] == 'l') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'm' && ai_wholecode[i + 1] == 'M') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'M' && ai_wholecode[i + 1] == 'm') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'e' && ai_wholecode[i + 1] == 'E') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'E' && ai_wholecode[i + 1] == 'e') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 's' && ai_wholecode[i + 1] == 'S') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else if (ai_wholecode[i] == 'S' && ai_wholecode[i + 1] == 's') {
			ai_wholecode.erase(i, 2); i = 0;
		}
		else
			i++;
	}
	//double symbols reduce to '^'
	for (int i = 0; i < ai_wholecode.length();) {
		if (ai_wholecode[i] == 'u' && ai_wholecode[i + 1] == 'u') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'U' && ai_wholecode[i + 1] == 'U') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'd' && ai_wholecode[i + 1] == 'd') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'D' && ai_wholecode[i + 1] == 'D') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'f' && ai_wholecode[i + 1] == 'f') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'F' && ai_wholecode[i + 1] == 'F') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'r' && ai_wholecode[i + 1] == 'r') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'R' && ai_wholecode[i + 1] == 'R') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'b' && ai_wholecode[i + 1] == 'b') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'B' && ai_wholecode[i + 1] == 'B') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'l' && ai_wholecode[i + 1] == 'l') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'L' && ai_wholecode[i + 1] == 'L') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'm' && ai_wholecode[i + 1] == 'm') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'M' && ai_wholecode[i + 1] == 'M') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 's' && ai_wholecode[i + 1] == 's') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'S' && ai_wholecode[i + 1] == 'S') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'e' && ai_wholecode[i + 1] == 'e') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else if (ai_wholecode[i] == 'E' && ai_wholecode[i + 1] == 'E') {
			ai_wholecode[i + 1] = '^'; i = 0;
		}
		else
			i++;
	}
	return ai_wholecode;
}

int calc_length(string in) {
	int res = 0;
	for (int i = 0; i < in.length(); i++)
		if (in[i] != '^')
			res++;
	return res;
}