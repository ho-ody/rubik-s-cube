#include <iostream>
#include "Block.h"
using namespace std;

extern int N;
int*** sides;
extern int* order;
extern Block* GLOBALblocks;

extern int code_input_index;
extern string code_s;

void generateSidesTables() {
	sides = new int** [6];
	for (int i = 0; i < 6; i++) {
		sides[i] = new int* [N];
		for (int j = 0; j < N; j++) {
			sides[i][j] = new int[N];
		}
	}	
}

void letsGoAiStart() {
	generateSidesTables();
}
// (-1): none, (0): white, (1): yellow, (2):red, (3): orange, (4): blue, (5): green
// (0): down, (1): up, (2): right, (3): left, (4): front, (5): back
void updateSidesFromOrder() {
	for (int i = 0; i < N; i++) { //front + back
		for (int j = 0; j < N; j++) {
			sides[0][N - 1 - i][N - 1 - j] = GLOBALblocks[order[(i * N + j) % N + ((i * N + j) / N) * N * N]].color[5];//down
			sides[1][i][N - 1 - j] = GLOBALblocks[order[(i * N + j) % N + ((i * N + j) / N) * N * N + N * (N - 1)]].color[4];//up

			sides[2][i][j] = GLOBALblocks[order[((i * N + j) % N) * N * N + ((i * N + j) / N) * N ]].color[3];//right
			sides[3][i][N - 1 - j] = GLOBALblocks[order[((i * N + j) % N) * N * N + ((i * N + j) / N) * N + (N - 1)]].color[1];//left

			sides[4][i][N - 1 - j] = GLOBALblocks[order[i * N + j]].color[0];//front
			sides[5][i][j] = GLOBALblocks[order[i * N + j + N * N * (N - 1)]].color[2];//back
		}
	}
}

void showSides() {
	string names[6] = { "down","up","right","left","front","back" };
	cerr << names[1] << endl;
	for (int i = N - 1; i >= 0; i--) { //up
		for (int j = 0; j < N; j++) {
			cerr << sides[1][i][j] << " ";
		}
		cerr << "\n";
	}
	int little_order[] = { 4,2,5,3 };
	for (int i = 0; i < 4; i++) {
		cerr << names[little_order[i]] << "\t";
	}
	cerr << "\n";
	for (int i = N-1; i >=0; i--) {
		for (int k = 0; k < 4; k++) {
			for (int j = 0; j < N; j++) {
				cerr << sides[little_order[k]][i][j] << " ";
			}
			cerr << "\t";
		}
		cerr << "\n";
	}
	cerr << names[0] << endl;
	for (int i = N - 1; i >= 0; i--) { //down
		for (int j = 0; j < N; j++) {
			cerr << sides[0][i][j] << " ";
		}
		cerr << "\n";
	}
	cerr << "\n";
}
int cross();
int f2l();
extern int ai_go;
void letsGoAiLoop() {
	updateSidesFromOrder();
	if (ai_go) {
		


		if (code_input_index == -1)
			f2l();
		//if (code_input_index == -1)
		//	if (cross() == -1)
		//		f2l();
		//showSides();
	}
}
void delete3dArray(int*** tab) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			delete[] tab[i][j];
		}
		delete[] tab[i];
	}	
	delete[] tab;
}
void letsGoAiEnd() {
	delete3dArray(sides);
}

//	x 3 x
//	1 x 2
//	x 0 x

string x1_pos_color_test(int new_pos, int new_color, int pos, int color) {
	//offset of color to other color lol
	int color_rules[6][6] = { {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,2,1,3}, {0,0,2,0,3,1}, {0,0,3,1,0,2}, {0,0,1,3,2,0} };
	int pos_rules[4][4] = { {0,3,1,2}, {1,0,2,3}, {3,2,0,1}, {2,1,3,0}};

	int move = color_rules[new_color][color] - pos_rules[new_pos][pos];

	if (move == 0)
		return "";
	if (move == 1)
		return "D";
	if (move == 2 || move == -2)
		return "dd";
	if (move == -1 || move == 3)
		return "d";
	cerr << "error_x1_pos_color_test!\n";
	return "";
}
string x3_pos_color_test(int new_pos, int new_color, int pos1, int color1, int pos2, int color2, int pos3, int color3) {
	int color_rules[6][6] = { {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,2,1,3}, {0,0,2,0,3,1}, {0,0,3,1,0,2}, {0,0,1,3,2,0} };
	int pos_rules[4][4] = { {0,3,1,2}, {1,0,2,3}, {3,2,0,1}, {2,1,3,0} };

	int test_1_2 = color_rules[color1][color2] - pos_rules[pos1][pos2];
	int test_2_3 = color_rules[color2][color3] - pos_rules[pos2][pos3];
	int test_1_3 = color_rules[color1][color3] - pos_rules[pos1][pos3];

	if (test_1_2 == 0 || test_1_3 == 0) {//jedynka na pewno git
		return x1_pos_color_test(new_pos, new_color, pos1, color1);
	}
	else {//jedynka na pewno Ÿle
		return x1_pos_color_test(new_pos, new_color, pos2, color2);
	}
	return "";
}

int distance_p(int pos1, int pos2) {
	int pos_rules[4][4] = { {0,3,1,2}, {1,0,2,3}, {3,2,0,1}, {2,1,3,0} };
	return pos_rules[pos1][pos2];
}

int distance_c(int color1, int color2) {
	int color_rules[6][6] = { {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,2,1,3}, {0,0,2,0,3,1}, {0,0,3,1,0,2}, {0,0,1,3,2,0} };
	//cerr << "d= " << color_rules[color1][color2] << endl;
	return color_rules[color1][color2];
}

string turnTopToMatchCorner(int distance) {
	if (distance == 0)
		return "";
	if (distance == 1)
		return "U";
	if (distance == 2)
		return "uu";
	if (distance == 3)
		return "u";
}
string turnBottomToMatchCorner(int distance) {
	if (distance == 0)
		return "";
	if (distance == 1)
		return "D";
	if (distance == 2)
		return "dd";
	if (distance == 3)
		return "d";
}

enum sides_code { down = 0, up = 1, right = 2, left = 3, front = 4, back = 5, d = 0, u = 1, r = 2, l = 3, f = 4, b = 5 };
// (0): down, (1): up, (2): right, (3): left, (4): front, (5): back
int cross() {
	//cerr << "cross!\n";
	string superFlip = "rflbrd";
	string baseMove = "";

	if (sides[f][0][1] == 0 && sides[r][0][1] == 0 && sides[b][0][1] == 0 && sides[l][0][1] == 0 &&
		sides[d][0][1] == 5 && sides[d][1][0] == 3 && sides[d][1][2] == 2 && sides[d][2][1] == 4) {
			cerr << "superFlip!\n";
			code_s = superFlip;
			code_input_index = 0;
		}
	//first type of move
	else if (sides[f][1][0] == 0 || sides[f][1][2] == 0 || sides[r][1][0] == 0 || sides[r][1][2] == 0 ||
		sides[b][1][0] == 0 || sides[b][1][2] == 0 || sides[l][1][0] == 0 || sides[l][1][2] == 0) { 
		//base move + colorID + finalPosition
		// finalPosition:
		//	x 3 x
		//	1 x 2
		//	x 0 x
		int colorID, finalPosition;
		if (sides[f][1][0] == 0) {
			baseMove = "l";
			colorID = sides[l][1][2];
			finalPosition = 1;
		}	
		else if (sides[f][1][2] == 0) {
			baseMove = "R";
			colorID = sides[r][1][0];
			finalPosition = 2;
		}
		else if (sides[r][1][0] == 0) {
			baseMove = "f";
			colorID = sides[f][1][2];
			finalPosition = 3;
		}
		else if (sides[r][1][2] == 0) {
			baseMove = "B";
			colorID = sides[b][1][0];
			finalPosition = 0;
		}
		else if (sides[b][1][0] == 0) {
			baseMove = "r";
			colorID = sides[r][1][2];
			finalPosition = 2;
		}
		else if (sides[b][1][2] == 0) {
			baseMove = "L";
			colorID = sides[l][1][0];
			finalPosition = 1;
		}
		else if (sides[l][1][0] == 0) {
			baseMove = "b";
			colorID = sides[b][1][2];
			finalPosition = 0;
		}
		else if (sides[l][1][2] == 0) {
			baseMove = "F";
			colorID = sides[f][1][0];
			finalPosition = 3;
		}
		//check number of blocks already filled in bottom
		int blocksOnBottomCount = 0;
		int blocksOnBottom[4] = { -1,-1,-1,-1 };
		if (sides[d][0][1] == 0) {
			blocksOnBottomCount++; blocksOnBottom[0] = sides[b][0][1];
		}
		if (sides[d][1][0] == 0) {
			blocksOnBottomCount++; blocksOnBottom[1] = sides[l][0][1];
		}
		if (sides[d][1][2] == 0) {
			blocksOnBottomCount++; blocksOnBottom[2] = sides[r][0][1];
		}
		if (sides[d][2][1] == 0) {
			blocksOnBottomCount++; blocksOnBottom[3] = sides[f][0][1];
		}
		//if bottom is empty
		if (blocksOnBottomCount == 0) {
			code_s = baseMove;
			code_input_index = 0;
		}
		//1 block already filled
		//2 blocks already filled -> w sumie to nie ma ró¿nicy, jak s¹ dobrze to i tak siê dopasuje, jak nie to wywali jeden z³y najwy¿ej
		else if (blocksOnBottomCount == 1 || blocksOnBottomCount == 2) {
			int pos, color;
			if (blocksOnBottom[0] != -1)
				pos = 0;
			else if (blocksOnBottom[1] != -1)
				pos = 1;
			else if (blocksOnBottom[2] != -1)
				pos = 2; 
			else
				pos = 3;

			color = blocksOnBottom[pos];
			code_s = x1_pos_color_test(finalPosition, colorID, pos, color) + baseMove;
			//cerr << x1_pos_color_test(finalPosition, colorID, pos, color) + baseMove << endl;
			code_input_index = 0;
		}
		//3 blocks already filled
		else if (blocksOnBottomCount == 3) {
			int pos1 = -1, pos2 = -1, pos3 = -1, color1, color2, color3;
			if (blocksOnBottom[0] != -1)
				if (pos1 == -1) pos1 = 0; else if (pos2 == -1) pos2 = 0; else pos3 = 0;
			if (blocksOnBottom[1] != -1)
				if (pos1 == -1) pos1 = 1; else if (pos2 == -1) pos2 = 1; else pos3 = 1;
			if (blocksOnBottom[2] != -1)
				if (pos1 == -1) pos1 = 2; else if (pos2 == -1) pos2 = 2; else pos3 = 2;
			if (blocksOnBottom[3] != -1)
				if (pos1 == -1) pos1 = 3; else if (pos2 == -1) pos2 = 3; else pos3 = 3;

			color1 = blocksOnBottom[pos1];
			color2 = blocksOnBottom[pos2];
			color3 = blocksOnBottom[pos3];
			code_s = x3_pos_color_test(finalPosition, colorID, pos1, color1, pos2, color2, pos3, color3) + baseMove;
			code_input_index = 0;
		}
	}
	//second type of move
	else if (sides[u][0][1] == 0 || sides[u][1][0] == 0 || sides[u][1][2] == 0 || sides[u][2][1] == 0 ) {
		int colorID, finalPosition;
		if (sides[u][0][1] == 0) {
			baseMove = "ff";
			colorID = sides[f][2][1];
			finalPosition = 3;
		}
		else if (sides[u][1][0] == 0) {
			baseMove = "ll";
			colorID = sides[l][2][1];
			finalPosition = 1;
		}
		else if (sides[u][1][2] == 0) {
			baseMove = "rr";
			colorID = sides[r][2][1];
			finalPosition = 2;
		}
		else if (sides[u][2][1] == 0) {
			baseMove = "bb";
			colorID = sides[b][2][1];
			finalPosition = 0;
		}
		//check number of blocks already filled in bottom
		int blocksOnBottomCount = 0;
		int blocksOnBottom[4] = { -1,-1,-1,-1 };
		if (sides[d][0][1] == 0) {
			blocksOnBottomCount++; blocksOnBottom[0] = sides[b][0][1];
		}
		if (sides[d][1][0] == 0) {
			blocksOnBottomCount++; blocksOnBottom[1] = sides[l][0][1];
		}
		if (sides[d][1][2] == 0) {
			blocksOnBottomCount++; blocksOnBottom[2] = sides[r][0][1];
		}
		if (sides[d][2][1] == 0) {
			blocksOnBottomCount++; blocksOnBottom[3] = sides[f][0][1];
		}
		//if bottom is empty
		if (blocksOnBottomCount == 0) {
			code_s = baseMove;
			code_input_index = 0;
		}
		//1 block already filled
		//2 blocks already filled
		else if (blocksOnBottomCount == 1 || blocksOnBottomCount == 2) {
			int pos, color;
			if (blocksOnBottom[0] != -1)
				pos = 0;
			else if (blocksOnBottom[1] != -1)
				pos = 1;
			else if (blocksOnBottom[2] != -1)
				pos = 2;
			else
				pos = 3;

			color = blocksOnBottom[pos];
			code_s = x1_pos_color_test(finalPosition, colorID, pos, color) + baseMove;
			//cerr << x1_pos_color_test(finalPosition, colorID, pos, color) + baseMove << endl;
			code_input_index = 0;
		}
		//3 blocks already filled
		else if (blocksOnBottomCount == 3) {
			int pos1 = -1, pos2 = -1, pos3 = -1, color1, color2, color3;
			if (blocksOnBottom[0] != -1)
				if (pos1 == -1) pos1 = 0; else if (pos2 == -1) pos2 = 0; else pos3 = 0;
			if (blocksOnBottom[1] != -1)
				if (pos1 == -1) pos1 = 1; else if (pos2 == -1) pos2 = 1; else pos3 = 1;
			if (blocksOnBottom[2] != -1)
				if (pos1 == -1) pos1 = 2; else if (pos2 == -1) pos2 = 2; else pos3 = 2;
			if (blocksOnBottom[3] != -1)
				if (pos1 == -1) pos1 = 3; else if (pos2 == -1) pos2 = 3; else pos3 = 3;

			color1 = blocksOnBottom[pos1];
			color2 = blocksOnBottom[pos2];
			color3 = blocksOnBottom[pos3];
			code_s = x3_pos_color_test(finalPosition, colorID, pos1, color1, pos2, color2, pos3, color3) + baseMove;
			code_input_index = 0;
		}
	}
	//third type of move
	else if (sides[f][2][1] == 0 || sides[r][2][1] == 0 || sides[b][2][1] == 0 || sides[l][2][1] == 0) {
		int colorID, finalPosition;
		if (sides[f][2][1] == 0) {
			baseMove = "URfr";
			colorID = sides[u][0][1];
			finalPosition = 3;
		}
		else if (sides[l][2][1] == 0) {
			baseMove = "UFlf";
			colorID = sides[u][1][0];
			finalPosition = 1;
		}
		else if (sides[r][2][1] == 0) {
			baseMove = "UBrb";
			colorID = sides[u][1][2];
			finalPosition = 2;
		}
		else if (sides[b][2][1] == 0) {
			baseMove = "ULbl";
			colorID = sides[u][1][0];
			finalPosition = 0;
		}
		//check number of blocks already filled in bottom
		int blocksOnBottomCount = 0;
		int blocksOnBottom[4] = { -1,-1,-1,-1 };
		if (sides[d][0][1] == 0) {
			blocksOnBottomCount++; blocksOnBottom[0] = sides[b][0][1];
		}
		if (sides[d][1][0] == 0) {
			blocksOnBottomCount++; blocksOnBottom[1] = sides[l][0][1];
		}
		if (sides[d][1][2] == 0) {
			blocksOnBottomCount++; blocksOnBottom[2] = sides[r][0][1];
		}
		if (sides[d][2][1] == 0) {
			blocksOnBottomCount++; blocksOnBottom[3] = sides[f][0][1];
		}
		//if bottom is empty
		if (blocksOnBottomCount == 0) {
			code_s = baseMove;
			code_input_index = 0;
		}
		//1 block already filled
		//2 blocks already filled
		else if (blocksOnBottomCount == 1 || blocksOnBottomCount == 2) {
			int pos, color;
			if (blocksOnBottom[0] != -1)
				pos = 0;
			else if (blocksOnBottom[1] != -1)
				pos = 1;
			else if (blocksOnBottom[2] != -1)
				pos = 2;
			else
				pos = 3;

			color = blocksOnBottom[pos];
			code_s = x1_pos_color_test(finalPosition, colorID, pos, color) + baseMove;
			//cerr << x1_pos_color_test(finalPosition, colorID, pos, color) + baseMove << endl;
			code_input_index = 0;
		}
		//3 blocks already filled
		else if (blocksOnBottomCount == 3) {
			int pos1 = -1, pos2 = -1, pos3 = -1, color1, color2, color3;
			if (blocksOnBottom[0] != -1)
				if (pos1 == -1) pos1 = 0; else if (pos2 == -1) pos2 = 0; else pos3 = 0;
			if (blocksOnBottom[1] != -1)
				if (pos1 == -1) pos1 = 1; else if (pos2 == -1) pos2 = 1; else pos3 = 1;
			if (blocksOnBottom[2] != -1)
				if (pos1 == -1) pos1 = 2; else if (pos2 == -1) pos2 = 2; else pos3 = 2;
			if (blocksOnBottom[3] != -1)
				if (pos1 == -1) pos1 = 3; else if (pos2 == -1) pos2 = 3; else pos3 = 3;

			color1 = blocksOnBottom[pos1];
			color2 = blocksOnBottom[pos2];
			color3 = blocksOnBottom[pos3];
			code_s = x3_pos_color_test(finalPosition, colorID, pos1, color1, pos2, color2, pos3, color3) + baseMove;
			code_input_index = 0;
		}
	}
	//forth type of move
	else if (sides[f][0][1] == 0 || sides[r][0][1] == 0 || sides[b][0][1] == 0 || sides[l][0][1] == 0) {
		if (sides[f][0][1] == 0) {
			baseMove = "f";
		}
		else if (sides[l][0][1] == 0) {
			baseMove = "l";
		}
		else if (sides[r][0][1] == 0) {
			baseMove = "r";
		}
		else if (sides[b][0][1] == 0) {
			baseMove = "b";
		}
		code_s = baseMove;
		code_input_index = 0;
	}
	//fix bottom orientation
	else if (sides[f][0][1] != sides[f][1][1]) {
		baseMove = turnBottomToMatchCorner(distance_c(sides[f][1][1], sides[f][0][1]));
		code_s = baseMove;
		code_input_index = 0;
	}
	return code_input_index;
}
//	x 3 x	1 x 3	1 3 3
//	1 x 2	x x x	1 x 2
//	x 0 x	0 x 2	0 0 2
// (-1): none, (0): white, (1): yellow, (2):red, (3): orange, (4): blue, (5): green
int f2l() {
	//cerr << "f2l!\n";
	string baseMove = "";
	//1st: Easy cases
	if ((sides[r][2][0] == 0 && sides[u][2][1] == sides[f][2][2] && sides[b][2][1] == sides[u][0][2]) ||
		(sides[b][2][0] == 0 && sides[u][1][0] == sides[r][2][2] && sides[l][2][1] == sides[u][2][2]) ||
		(sides[l][2][0] == 0 && sides[u][0][1] == sides[b][2][2] && sides[f][2][1] == sides[u][2][0]) ||
		(sides[f][2][0] == 0 && sides[u][1][2] == sides[l][2][2] && sides[r][2][1] == sides[u][0][0])) {
		//rotate up to be above right place
		string moveBasedOnColor[] = { "","","buB","fuL","ruR","luL" };
		if (sides[r][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[f][2][2], sides[f][1][1]));
			baseMove += moveBasedOnColor[sides[f][2][2]];
		}
		else if (sides[b][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[r][2][2], sides[r][1][1]));
			baseMove += moveBasedOnColor[sides[r][2][2]];
		}
		else if (sides[l][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[b][2][2], sides[b][1][1]));
			baseMove += moveBasedOnColor[sides[b][2][2]];
		}
		else if (sides[f][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[l][2][2], sides[l][1][1]));
			baseMove += moveBasedOnColor[sides[l][2][2]];
		}
		code_s = baseMove;
		code_input_index = 0;
		//cerr << "a) ";
	}
	else if ((sides[f][2][2] == 0 && sides[u][1][0] == sides[r][2][0] && sides[l][2][1] == sides[u][0][2]) ||
		(sides[r][2][2] == 0 && sides[u][0][1] == sides[b][2][0] && sides[f][2][1] == sides[u][2][2]) ||
		(sides[b][2][2] == 0 && sides[u][1][2] == sides[l][2][0] && sides[r][2][1] == sides[u][2][0]) ||
		(sides[l][2][2] == 0 && sides[u][2][1] == sides[f][2][0] && sides[b][2][1] == sides[u][0][0])) {
		//rotate up to be above right place
		string moveBasedOnColor[] = { "","","FUf","BUb","LUl","RUr" };
		if (sides[f][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[r][2][0], sides[r][1][1]));
			baseMove += moveBasedOnColor[sides[r][2][0]];
		}
		else if (sides[r][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[b][2][0], sides[b][1][1]));
			baseMove += moveBasedOnColor[sides[b][2][0]];
		}
		else if (sides[b][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[l][2][0], sides[l][1][1]));
			baseMove += moveBasedOnColor[sides[l][2][0]];
		}
		else if (sides[l][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[f][2][0], sides[f][1][1]));
			baseMove += moveBasedOnColor[sides[f][2][0]];
		}
		code_s = baseMove;
		code_input_index = 0;
		//cerr << "b) ";
	}
	else if ((sides[r][2][0] == 0 && sides[f][2][1] == sides[f][2][2] && sides[u][0][1] == sides[u][0][2]) ||
		(sides[b][2][0] == 0 && sides[r][2][1] == sides[r][2][2] && sides[u][1][2] == sides[u][2][2]) ||
		(sides[l][2][0] == 0 && sides[b][2][1] == sides[b][2][2] && sides[u][2][1] == sides[u][2][0]) ||
		(sides[f][2][0] == 0 && sides[l][2][1] == sides[l][2][2] && sides[u][1][0] == sides[u][0][0])) {
		//rotate up to be above right place
		string moveBasedOnColor[] = { "","","URur","ULul","UFuf","UBub" };
		if (sides[r][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[f][2][2], sides[f][1][1]));
			baseMove += moveBasedOnColor[sides[f][2][2]];
		}
		else if (sides[b][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[r][2][2], sides[r][1][1]));
			baseMove += moveBasedOnColor[sides[r][2][2]];
		}
		else if (sides[l][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[b][2][2], sides[b][1][1]));
			baseMove += moveBasedOnColor[sides[b][2][2]];
		}
		else if (sides[f][2][0] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[l][2][2], sides[l][1][1]));
			baseMove += moveBasedOnColor[sides[l][2][2]];
		}
		code_s = baseMove;
		code_input_index = 0;
		//cerr << "c) ";
	}
	else if ((sides[f][2][2] == 0 && sides[r][2][1] == sides[r][2][0] && sides[u][1][2] == sides[u][0][2]) ||
		(sides[r][2][2] == 0 && sides[b][2][1] == sides[b][2][0] && sides[u][2][1] == sides[u][2][2]) ||
		(sides[b][2][2] == 0 && sides[l][2][1] == sides[l][2][0] && sides[u][1][0] == sides[u][2][0]) ||
		(sides[l][2][2] == 0 && sides[f][2][1] == sides[f][2][0] && sides[u][0][1] == sides[u][0][0])) {
		//rotate up to be above right place
		string moveBasedOnColor[] = { "","","urUR","ulUL","ufUF","ubUB" };
		if (sides[f][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[r][2][0], sides[r][1][1]));
			baseMove += moveBasedOnColor[sides[r][2][0]];
		}
		else if (sides[r][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[b][2][0], sides[b][1][1]));
			baseMove += moveBasedOnColor[sides[b][2][0]];
		}
		else if (sides[b][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[l][2][0], sides[l][1][1]));
			baseMove += moveBasedOnColor[sides[l][2][0]];
		}
		else if (sides[l][2][2] == 0) {
			baseMove = turnTopToMatchCorner(distance_c(sides[f][2][0], sides[f][1][1]));
			baseMove += moveBasedOnColor[sides[f][2][0]];
		}
		code_s = baseMove;
		code_input_index = 0;
		//cerr << "d) ";
	}

	//cerr << code_s << endl;
	//code_input_index = -1;

	return code_input_index;
}
