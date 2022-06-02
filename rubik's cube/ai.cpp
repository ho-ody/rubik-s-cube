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
int oll();
extern int ai_go;
extern int rotateCounter;
int ok_its_enought = 0;
void letsGoAiLoop() {
	updateSidesFromOrder();
	if (ai_go) {
		


		//if (code_input_index == -1)
		//	f2l();
		//if (code_input_index == -1 && rotateCounter < 0)
		//	if (cross() == -1)
		//		if (f2l() == -1)
					if (ok_its_enought == false)
						ok_its_enought = oll();
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
// (0): white, (1): yellow, (2): red,   (3): orange, (4): blue,  (5): green
// (0): down,  (1): up,     (2): right, (3): left,   (4): front, (5): back
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

int distance_edge_corner(int a, int b) {
	int t = a % 8 - b % 8;
	if (t < 0)
		t += 4;
	return t;
} 
//	x 3 x	1 x 3	1 3 3
//	1 x 2	x x x	1 x 2
//	x 0 x	0 x 2	0 0 2
// 
// (0): white, (1): yellow, (2): red,   (3): orange, (4): blue,  (5): green
// (0): down,  (1): up,     (2): right, (3): left,   (4): front, (5): back
int f2l() {
	//cerr << "f2l!\n";
	string baseMove = "";
	//Positions of corners and edges
	int corners[4]; //fr, rb, bl, lf	// 0 -> 8  8 -> 15  15 -> 23	w zale¿noœci od rotacji
	int edges[4];   //fr, rb, bl, lf	// 0 -> 7  8 -> 15				w zale¿noœci od rotacji
	//edges
	int edges_colors[8][2] = { {sides[f][1][2],sides[r][1][0]}, {sides[r][1][2],sides[b][1][0]}, {sides[b][1][2],sides[l][1][0]}, {sides[l][1][2],sides[f][1][0]},{sides[f][2][1],sides[u][0][1]}, {sides[r][2][1],sides[u][1][2]}, {sides[b][2][1],sides[u][2][1]}, {sides[l][2][1],sides[u][1][0]} };
	for (int i = 0; i < 8; i++) {
		if (edges_colors[i][0] == r) {
			if (edges_colors[i][1] == b)
				edges[1] = i;
			else if (edges_colors[i][1] == f)
				edges[0] = i + 8;
		}
		else if (edges_colors[i][0] == b) {
			if (edges_colors[i][1] == r)
				edges[1] = i + 8;
			else if (edges_colors[i][1] == l)
				edges[2] = i;
		}
		else if (edges_colors[i][0] == l) {
			if (edges_colors[i][1] == f)
				edges[3] = i;
			else if (edges_colors[i][1] == b)
				edges[2] = i + 8;
		}
		else if (edges_colors[i][0] == f) {
			if (edges_colors[i][1] == l)
				edges[3] = i + 8;
			else if (edges_colors[i][1] == r)
				edges[0] = i;
		}
	}
	//corners
	int corners_colors[8][3] = { {sides[f][0][2], sides[r][0][0], sides[d][2][2]}, {sides[r][0][2], sides[b][0][0], sides[d][0][2]}, {sides[b][0][2], sides[l][0][0], sides[d][0][0]}, {sides[l][0][2], sides[f][0][0], sides[d][2][0]}, {sides[u][0][2], sides[r][2][0], sides[f][2][2]}, {sides[u][2][2], sides[b][2][0], sides[r][2][2]},{sides[u][2][0], sides[l][2][0], sides[b][2][2]},{sides[u][0][0], sides[f][2][0], sides[l][2][2]} };
	for (int i = 0; i < 8; i++) {
		if (corners_colors[i][0] == f) {
			if (corners_colors[i][1] == r && corners_colors[i][2] == 0)
				corners[0] = i;
			else if (corners_colors[i][1] == d && corners_colors[i][2] == l)
				corners[3] = i + 16;
		}
		if (corners_colors[i][0] == r) {
			if (corners_colors[i][1] == b && corners_colors[i][2] == 0)
				corners[1] = i;
			else if (corners_colors[i][1] == d && corners_colors[i][2] == f)
				corners[0] = i + 16;
		}
		if (corners_colors[i][0] == b) {
			if (corners_colors[i][1] == l && corners_colors[i][2] == 0)
				corners[2] = i;
			else if (corners_colors[i][1] == d && corners_colors[i][2] == r)
				corners[1] = i + 16;
		}
		if (corners_colors[i][0] == l) {
			if (corners_colors[i][1] == f && corners_colors[i][2] == 0)
				corners[3] = i;
			else if (corners_colors[i][1] == d && corners_colors[i][2] == b)
				corners[2] = i + 16;
		}
		if (corners_colors[i][0] == d) {
			if (corners_colors[i][1] == f && corners_colors[i][2] == r)
				corners[0] = i + 8;
			else if (corners_colors[i][1] == r && corners_colors[i][2] == b)
				corners[1] = i + 8;
			else if (corners_colors[i][1] == b && corners_colors[i][2] == l)
				corners[2] = i + 8;
			else if (corners_colors[i][1] == l && corners_colors[i][2] == f)
				corners[3] = i + 8;
		}
	}
	//for (int i = 0; i < 4; i++) cerr << corners[i] << ", ";
	//cerr << endl;


	//1st: Easy cases
	if ((corners[0] / 8 == 2 && edges[0] / 8 == 1 && distance_edge_corner(corners[0], edges[0]) == 2 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4) ||
		(corners[1] / 8 == 2 && edges[1] / 8 == 1 && distance_edge_corner(corners[1], edges[1]) == 2 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4) ||
		(corners[2] / 8 == 2 && edges[2] / 8 == 1 && distance_edge_corner(corners[2], edges[2]) == 2 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4) ||
		(corners[3] / 8 == 2 && edges[3] / 8 == 1 && distance_edge_corner(corners[3], edges[3]) == 2 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)) { //1-1
		
		int pairID;
		if (corners[0] / 8 == 2 && edges[0] / 8 == 1 && distance_edge_corner(corners[0], edges[0]) == 2 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4)
			pairID = 0;
		else if (corners[1] / 8 == 2 && edges[1] / 8 == 1 && distance_edge_corner(corners[1], edges[1]) == 2 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4)
			pairID = 1;
		else if (corners[2] / 8 == 2 && edges[2] / 8 == 1 && distance_edge_corner(corners[2], edges[2]) == 2 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4)
			pairID = 2;
		else if (corners[3] / 8 == 2 && edges[3] / 8 == 1 && distance_edge_corner(corners[3], edges[3]) == 2 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)
			pairID = 3;

		string moves[] = {"ruR","buB","luL","fuF"};
		int preoffset = 0;
		preoffset += corners[pairID] % 8 - 4 - pairID;
		if (preoffset > 3)
			preoffset -= 4;
		if (preoffset < 0)
			preoffset += 4;
		string move = "";
		string premove = "";
		switch (preoffset) {
		case 0:
			premove = "";
			break;
		case 1:
			premove = "u";
			break;
		case 2:
			premove = "uu";
			break;
		case 3:
			premove = "U";
			break;
		}

		move = premove + moves[pairID];
		cerr << "1-1[" << pairID << "]\t";
		code_s = move;
		code_input_index = 0;
	}
	else if ((corners[0] / 8 == 2 && edges[0] / 8 == 0 && distance_edge_corner(corners[0], edges[0]) == 0 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4) ||
		(corners[1] / 8 == 2 && edges[1] / 8 == 0 && distance_edge_corner(corners[1], edges[1]) == 0 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4) ||
		(corners[2] / 8 == 2 && edges[2] / 8 == 0 && distance_edge_corner(corners[2], edges[2]) == 0 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4) ||
		(corners[3] / 8 == 2 && edges[3] / 8 == 0 && distance_edge_corner(corners[3], edges[3]) == 0 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)) { //1-3

		int pairID;
		if (corners[0] / 8 == 2 && edges[0] / 8 == 0 && distance_edge_corner(corners[0], edges[0]) == 0 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4)
			pairID = 0;
		else if (corners[1] / 8 == 2 && edges[1] / 8 == 0 && distance_edge_corner(corners[1], edges[1]) == 0 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4)
			pairID = 1;
		else if (corners[2] / 8 == 2 && edges[2] / 8 == 0 && distance_edge_corner(corners[2], edges[2]) == 0 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4)
			pairID = 2;
		else if (corners[3] / 8 == 2 && edges[3] / 8 == 0 && distance_edge_corner(corners[3], edges[3]) == 0 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)
			pairID = 3;

		string moves[] = { "UFuf","URur","UBub","ULul" };
		int preoffset = 0;
		preoffset += corners[pairID] % 8 - 4 - pairID;
		if (preoffset > 3)
			preoffset -= 4;
		if (preoffset < 0)
			preoffset += 4;
		string move = "";
		string premove = "";
		switch (preoffset) {
		case 0:
			premove = "";
			break;
		case 1:
			premove = "u";
			break;
		case 2:
			premove = "uu";
			break;
		case 3:
			premove = "U";
			break;
		}

		move = premove + moves[pairID];
		cerr << "1-3[" << pairID << "]\t";
		code_s = move;
		code_input_index = 0;
	}
	else if ((corners[0] / 8 == 0 && edges[0] / 8 == 0 && distance_edge_corner(corners[0], edges[0]) == 1 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4) ||
		(corners[1] / 8 == 0 && edges[1] / 8 == 0 && distance_edge_corner(corners[1], edges[1]) == 1 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4) ||
		(corners[2] / 8 == 0 && edges[2] / 8 == 0 && distance_edge_corner(corners[2], edges[2]) == 1 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4) ||
		(corners[3] / 8 == 0 && edges[3] / 8 == 0 && distance_edge_corner(corners[3], edges[3]) == 1 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)) { //1-2

		int pairID;
		if (corners[0] / 8 == 0 && edges[0] / 8 == 0 && distance_edge_corner(corners[0], edges[0]) == 1 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4)
			pairID = 0;
		else if (corners[1] / 8 == 0 && edges[1] / 8 == 0 && distance_edge_corner(corners[1], edges[1]) == 1 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4)
			pairID = 1;
		else if (corners[2] / 8 == 0 && edges[2] / 8 == 0 && distance_edge_corner(corners[2], edges[2]) == 1 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4)
			pairID = 2;
		else if (corners[3] / 8 == 0 && edges[3] / 8 == 0 && distance_edge_corner(corners[3], edges[3]) == 1 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)
			pairID = 3;

		string moves[] = { "FUf","RUr","BUb","LUl" };
		int preoffset = 0;
		preoffset += corners[pairID] % 8 - 4 - pairID;
		if (preoffset > 3)
			preoffset -= 4;
		if (preoffset < 0)
			preoffset += 4;
		string move = "";
		string premove = "";
		switch (preoffset) {
		case 0:
			premove = "";
			break;
		case 1:
			premove = "u";
			break;
		case 2:
			premove = "uu";
			break;
		case 3:
			premove = "U";
			break;
		}

		move = premove + moves[pairID];
		cerr << "1-2[" << pairID << "]\t";
		code_s = move;
		code_input_index = 0;
	}
	else if ((corners[0] / 8 == 0 && edges[0] / 8 == 1 && distance_edge_corner(corners[0], edges[0]) == 3 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4) ||
		(corners[1] / 8 == 0 && edges[1] / 8 == 1 && distance_edge_corner(corners[1], edges[1]) == 3 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4) ||
		(corners[2] / 8 == 0 && edges[2] / 8 == 1 && distance_edge_corner(corners[2], edges[2]) == 3 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4) ||
		(corners[3] / 8 == 0 && edges[3] / 8 == 1 && distance_edge_corner(corners[3], edges[3]) == 3 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)) { //1-4

		int pairID;
		if (corners[0] / 8 == 0 && edges[0] / 8 == 1 && distance_edge_corner(corners[0], edges[0]) == 3 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4)
			pairID = 0;
		else if (corners[1] / 8 == 0 && edges[1] / 8 == 1 && distance_edge_corner(corners[1], edges[1]) == 3 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4)
			pairID = 1;
		else if (corners[2] / 8 == 0 && edges[2] / 8 == 1 && distance_edge_corner(corners[2], edges[2]) == 3 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4)
			pairID = 2;
		else if (corners[3] / 8 == 0 && edges[3] / 8 == 1 && distance_edge_corner(corners[3], edges[3]) == 3 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)
			pairID = 3;

		string moves[] = { "urUR","ubUB","ulUL","ufUF" };
		int preoffset = 0;
		preoffset += corners[pairID] % 8 - 4 - pairID;
		if (preoffset > 3)
			preoffset -= 4;
		if (preoffset < 0)
			preoffset += 4;
		string move = "";
		string premove = "";
		switch (preoffset) {
		case 0:
			premove = "";
			break;
		case 1:
			premove = "u";
			break;
		case 2:
			premove = "uu";
			break;
		case 3:
			premove = "U";
			break;
		}

		move = premove + moves[pairID];
		cerr << "1-4[" << pairID << "]\t";
		code_s = move;
		code_input_index = 0;
	}
	//2nd case: Corner in bottom, edge in top layer
	else if ((corners[0] % 8 < 4 && edges[0] % 8 >= 4) || (corners[1] % 8 < 4 && edges[1] % 8 >= 4) || (corners[2] % 8 < 4 && edges[2] % 8 >= 4) || (corners[3] % 8 < 4 && edges[3] % 8 >= 4)) {
	int pairID;
	if (corners[0] % 8 < 4 && edges[0] % 8 >= 4)
		pairID = 0;
	else if (corners[1] % 8 < 4 && edges[1] % 8 >= 4)
		pairID = 1;
	else if (corners[2] % 8 < 4 && edges[2] % 8 >= 4)
		pairID = 2;
	else if (corners[3] % 8 < 4 && edges[3] % 8 >= 4)
		pairID = 3;
	
	string firstPhase[6][4] = {
		{"urUR","ubUB","ulUL","ufUF"},
		{"UFuf","URur","UBub","ULul"},
		{"Fuf","Rur","Bub","Lul"},
		{"ruR","buB","luL","fuF"},
		{"rUR","bUB","lUL","fUF"},
		{"FUf","RUr","BUb","LUl"}
	};
	string secondPhase[6][4] = {
		{"UFuf","URur","UBub","ULul"},
		{"urUR","ubUB","ulUL","ufUF"},
		{"UFuf","URur","UBub","ULul"},
		{"UruR","UbuB","UluL","UfuF"},
		{"urUR","ubUB","ulUL","ufUF"},
		{"uFUf","uRUr","uBUb","uLUl"}
	};
	string move = "";
	string midmove = "";
	string premove = "";
	int preoffset = 0;
	int typeOfCase = 0;
	if (corners[pairID] / 8 == 0) { //1+2
		if (edges[pairID] / 8 == 0) { //1
			typeOfCase = 1;
		}
		else { //2
			typeOfCase = 2;
			preoffset--;
		}
	}
	else if (corners[pairID] / 8 == 1) { //5+6
		if (edges[pairID] / 8 == 0) { //6
			typeOfCase = 6;	
		}
		else { //5
			typeOfCase = 5;
			preoffset--;
		}
	}
	else { //3+4
		if (edges[pairID] / 8 == 0) { //3
			typeOfCase = 3;
		}
		else { //4
			typeOfCase = 4;
			preoffset--;
		}
	}

	preoffset += edges[pairID] % 8 - corners[pairID] % 8;
	if (preoffset > 3)
		preoffset -= 4;

	switch (preoffset) {
	case 0:
		premove = "";
		break;
	case 1:
		premove = "u";
		break;
	case 2:
		premove = "uu";
		break;
	case 3:
		premove = "U";
		break;
	}
	int postoffset = corners[pairID] % 8 - pairID;
	int secondPhaseMove = corners[pairID] % 8;
	switch (postoffset) {
	case 0:
		midmove = "";
		break;
	case 1:
	case -3:
		midmove = "u";
		secondPhaseMove--;
		break;
	case -1:
	case 3:
		secondPhaseMove++;
		midmove = "U";
		break;
	case 2:
	case -2:
		secondPhaseMove -= 2;
		midmove = "uu";
		break;
	default:
		cerr << "error_postoffset_midmove: " << postoffset << endl;
	}
	if (secondPhaseMove > 3)
		secondPhaseMove -= 4;
	else if (secondPhaseMove < 0)
		secondPhaseMove += 4;

	//cerr << premove << "\t";
	//cerr << firstPhase[typeOfCase-1][corners[pairID] % 8] << "\t";
	//cerr << midmove << "\t";
	//cerr << secondPhase[typeOfCase-1][secondPhaseMove] << endl;
	cerr << "2-" << typeOfCase << "[" << pairID << "]\t";
	move = premove + firstPhase[typeOfCase - 1][corners[pairID] % 8] + midmove + secondPhase[typeOfCase - 1][secondPhaseMove];

	code_s = move;
	code_input_index = 0;
	}
	//3rd case: Corner in top, edge in middle
	else if ((corners[0] % 8 >= 4 && edges[0] % 8 < 4) || (corners[1] % 8 >= 4 && edges[1] % 8 < 4) || (corners[2] % 8 >= 4 && edges[2] % 8 < 4) || (corners[3] % 8 >= 4 && edges[3] % 8 < 4)) {
	int pairID;
	if (corners[0] % 8 >= 4 && edges[0] % 8 < 4)
		pairID = 0;
	else if (corners[1] % 8 >= 4 && edges[1] % 8 < 4)
		pairID = 1;
	else if (corners[2] % 8 >= 4 && edges[2] % 8 < 4)
		pairID = 2;
	else if (corners[3] % 8 >= 4 && edges[3] % 8 < 4)
		pairID = 3;

	string firstPhase[6][4] = {
		{"ruRUruRU","buBUbuBU","luLUluLU","fuFUfuFU"},
		{"rUR","bUB","lUL","fUF"},
		{"uFuf","uRur","uBub","uLul"},
		{"uFUf","uRUr","uBUb","uLUl"},
		{"UrUR","UbUB","UlUL","UfUF"},
		{"UruR","UbuB","UluL","UfuF"}
	};
	string secondPhase[6][4] = {
		{"ruR","buB","luL","fuF"},
		{"uFuf","uRur","uBub","uLul"}, //[d]->[U]+rot
		{"uFuuf","uRuur","uBuub","uLuul"},
		{"UruR","UbuB","UluL","UfuF"}, //[d']->[U']+rot
		{"UruuR","UbuuB","UluuL","UfuuF"},
		{"uFUf","uRUr","uBUb","uLUl"}
	};


	int typeOfCase = 0;
	int preoffset = 0;
	if (corners[pairID] / 8 == 0) { //5+6
		if (edges[pairID] / 8 == 0) { //5
			typeOfCase = 5;
		}
		else { //6
			typeOfCase = 6;
		}
	}
	else if (corners[pairID] / 8 == 1) { //1+2
		if (edges[pairID] / 8 == 0) { //1
			typeOfCase = 1;
		}
		else { //2
			typeOfCase = 2;
		}
	}
	else { //3+4
		if (edges[pairID] / 8 == 0) { //3
			typeOfCase = 3;
		}
		else { //4
			typeOfCase = 4;
		}
	}
	preoffset += corners[pairID] % 8 - edges[pairID] % 8;
	if (preoffset > 3)
		preoffset -= 4;
	string move = "";
	string midmove = "";
	string premove = "";
	switch (preoffset) {
	case 0:
		premove = "";
		break;
	case 1:
		premove = "u";
		break;
	case 2:
		premove = "uu";
		break;
	case 3:
		premove = "U";
		break;
	}

	int postoffset = edges[pairID] % 8 - pairID;
	int secondPhaseMove = edges[pairID] % 8;
	switch (postoffset) {
	case 0:
		midmove = "";
		break;
	case 1:
	case -3:
		midmove = "u";
		secondPhaseMove--;
		break;
	case -1:
	case 3:
		secondPhaseMove++;
		midmove = "U";
		break;
	case 2:
	case -2:
		secondPhaseMove -= 2;
		midmove = "uu";
		break;
	default:
		cerr << "error_postoffset_midmove: " << postoffset << endl;
	}
	if (secondPhaseMove > 3)
		secondPhaseMove -= 4;
	else if (secondPhaseMove < 0)
		secondPhaseMove += 4;

	//cerr << "("<< pairID << ") 3-" << typeOfCase << "  ->  " << premove << endl;
	//cerr << premove << "\t";
	//cerr << firstPhase[typeOfCase-1][edges[pairID] % 8] << "\t";
	//cerr << midmove << "\t";
	//cerr << secondPhase[typeOfCase-1][secondPhaseMove] << endl;
	cerr << "3-" << typeOfCase << "[" << pairID << "]\t";
	move = premove + firstPhase[typeOfCase - 1][edges[pairID] % 8] + midmove + secondPhase[typeOfCase - 1][secondPhaseMove];

	code_s = move;
	code_input_index = 0;
	}
	//4th case: Corner pointing outwards, edge in top layer
	//4b - parzyste: bia³y do osi Z
	else if ((corners[0] / 8 == 0 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4) || (corners[1] / 8 == 0 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4) || (corners[2] / 8 == 0 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4) || (corners[3] / 8 == 0 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)) {
	int pairID;
	if (corners[0] / 8 == 0 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4)
		pairID = 0;
	else if (corners[1] / 8 == 0 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4)
		pairID = 1;
	else if (corners[2] / 8 == 0 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4)
		pairID = 2;
	else if (corners[3] / 8 == 0 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)
		pairID = 3;

	string firstPhase[6][4] = {
		{"FufU","RurU","BubU","LulU"},
		{"UruuR","UbuuB","UluuL","UfuuF"},
		{"UruR","UbuB","UluL","UfuF"},
		{"uFufU","uRurU","uBubU","uLulU"},
		{"uFUfU","uRUrU","uBUbU","uLUlU"},
		{"UruuRu","UbuuBu","UluuLu","UfuuFu"}
	};
	string secondPhase[6][4] = {
		{"UruR","UbuB","UluL","UfuF"},
		{"UruuR","UbuuB","UluuL","UfuuF"},
		{"UruuR","UbuuB","UluuL","UfuuF"},
		{"FUf","RUr","BUb","LUl"},
		{"FUf","RUr","BUb","LUl"},
		{"FUf","RUr","BUb","LUl"}
	};

	int distance_edge_corner = corners[pairID] % 8 - edges[pairID] % 8;
	if (distance_edge_corner < 0)
		distance_edge_corner += 4;


	int typeOfCase = 0;
	int preoffset = 0;
	if (distance_edge_corner == 0) { //4+1
		if (edges[pairID] / 8 == 0) { //4
			typeOfCase = 4;
		}
		else { //1
			typeOfCase = 1;
		}
	}
	else if (distance_edge_corner == 1) { //?+2
		if (edges[pairID] / 8 == 0) { //?
			typeOfCase = -1;
			cerr << "error_4b_" << pairID << "_" << edges[pairID] << "_" << corners[pairID] << endl;
		}
		else { //2
			typeOfCase = 2;
		}
	}
	else if (distance_edge_corner == 2) { //5+3
		if (edges[pairID] / 8 == 0) { //5
			typeOfCase = 5;
		}
		else { //3
			typeOfCase = 3;
		}
	}
	else if (distance_edge_corner == 3) { //6+?
		if (edges[pairID] / 8 == 0) { //6
			typeOfCase = 6;
		}
		else { //?
			cerr << "error_4b_" << pairID << "_" << edges[pairID] << "_" << corners[pairID] << endl;
			typeOfCase = -1;
		}
	}
	preoffset += corners[pairID] % 8 - 4 - pairID;
	if (preoffset > 3)
		preoffset -= 4;
	if (preoffset < 0)
		preoffset += 4;
	string move = "";
	string premove = "";
	switch (preoffset) {
	case 0:
		premove = "";
		break;
	case 1:
		premove = "u";
		break;
	case 2:
		premove = "uu";
		break;
	case 3:
		premove = "U";
		break;
	}

	move = premove + firstPhase[typeOfCase-1][pairID] + secondPhase[typeOfCase-1][pairID];
	//move = premove;
	//cerr << "move = " << move << endl;
	//cerr << premove << "\t" << firstPhase[typeOfCase - 1][pairID] << "\t" << secondPhase[typeOfCase - 1][pairID] << endl;
	cerr << "4b-" << typeOfCase << "[" << pairID << "]\t";
	code_s = move;
	code_input_index = 0;
	}
	//4a - nieparzyste: bia³y do osi X
	else if ((corners[0] / 8 == 2 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4) || (corners[1] / 8 == 2 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4) || (corners[2] / 8 == 2 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4) || (corners[3] / 8 == 2 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)) {
	int pairID;
	if (corners[0] / 8 == 2 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4)
		pairID = 0;
	else if (corners[1] / 8 == 2 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4)
		pairID = 1;
	else if (corners[2] / 8 == 2 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4)
		pairID = 2;
	else if (corners[3] / 8 == 2 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)
		pairID = 3;

	string firstPhase[6][4] = {
		{"rURu","bUBu","lULu","fUFu"}, 
		{"uFuuf","uRuur","uBuub","uLuul"}, 
		{"uFUf","uRUr","uBUb","uLUl"}, 
		{"UrURu","UbUBu","UlULu","UfUFu"}, 
		{"UruRu","UbuBu","UluLu","UfuFu"}, 
		{"uFuufU","uRuurU","uBuubU","uLuulU"} 
	};
	string secondPhase[6][4] = {
		{"uFUf","uRUr","uBUb","uLUl"},
		{"uFuuf","uRuur","uBuub","uLuul"},
		{"uFuuf","uRuur","uBuub","uLuul"},
		{"ruR","buB","luL","fuF"},
		{"ruR","buB","luL","fuF"},
		{"ruR","buB","luL","fuF"},
	};

	int distance_edge_corner = corners[pairID] % 8 - edges[pairID] % 8;
	if (distance_edge_corner < 0)
		distance_edge_corner += 4;


	int typeOfCase = 0;
	int preoffset = 0;
	if (distance_edge_corner == 0) { //?+6
		if (edges[pairID] / 8 == 0) { //?
			typeOfCase = -1;
			cerr << "error_4a_" << pairID << "_" << edges[pairID] << "_" << corners[pairID] << endl;
		}
		else { //6
			typeOfCase = 6;
		}
	}
	else if (distance_edge_corner == 1) { //3+5
		if (edges[pairID] / 8 == 0) { //3
			typeOfCase = 3;
		}
		else { //5
			typeOfCase = 5;
		}
	}
	else if (distance_edge_corner == 2) { //2+?
		if (edges[pairID] / 8 == 0) { //2
			typeOfCase = 2;
		}
		else { //?
			typeOfCase = -1;
			cerr << "error_4a_" << pairID << "_" << edges[pairID] << "_" << corners[pairID] << endl;
		}
	}
	else if (distance_edge_corner == 3) { //1+4
		if (edges[pairID] / 8 == 0) { //1
			typeOfCase = 1;
		}
		else { //4
			typeOfCase = 4;
		}
	}
	preoffset += corners[pairID] % 8 - 4 - pairID;
	if (preoffset > 3)
		preoffset -= 4;
	if (preoffset < 0)
		preoffset += 4;
	string move = "";
	string premove = "";
	switch (preoffset) {
	case 0:
		premove = "";
		break;
	case 1:
		premove = "u";
		break;
	case 2:
		premove = "uu";
		break;
	case 3:
		premove = "U";
		break;
	}

	move = premove + firstPhase[typeOfCase - 1][pairID] + secondPhase[typeOfCase - 1][pairID];
	//cerr << premove << "\t" << firstPhase[typeOfCase - 1][pairID] << "\t" << secondPhase[typeOfCase - 1][pairID] << endl;
	cerr << "4a-" << typeOfCase << "[" << pairID << "]\t";
	code_s = move;
	code_input_index = 0;
	}
	//5th case: Corner pointing upwards, edge in top layer
	else if ((corners[0] / 8 == 1 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4) || (corners[1] / 8 == 1 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4) || (corners[2] / 8 == 1 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4) || (corners[3] / 8 == 1 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)) {
	int pairID;
	if (corners[0] / 8 == 1 && corners[0] % 8 >= 4 && edges[0] % 8 >= 4)
		pairID = 0;
	else if (corners[1] / 8 == 1 && corners[1] % 8 >= 4 && edges[1] % 8 >= 4)
		pairID = 1;
	else if (corners[2] / 8 == 1 && corners[2] % 8 >= 4 && edges[2] % 8 >= 4)
		pairID = 2;
	else if (corners[3] / 8 == 1 && corners[3] % 8 >= 4 && edges[3] % 8 >= 4)
		pairID = 3;

	string firstPhase[8][4] = {
		{"ruRUUruRU","buBUUbuBU","luLUUluLU","fuFUUfuFU"},
		{"FUfuuFUfu","RUruuRUru","BUbuuBUbu","LUluuLUlu"}, //y' -> [R] -> [F]
		{"uuruR","uubuB","uuluL","uufuF"},
		{"uuFUf","uuRUr","uuBUb","uuLUl"},
		{"uruuR","ubuuB","uluuL","ufuuF"},
		{"UFuuf","URuur","UBuub","ULuul"},
		{"ruuR","buuB","luuL","fuuF"},
		{"Fuuf","Ruur","Buub","Luul"}
	};
	string secondPhase[8][4] = {
		{"ruR","buB","luL","fuF"},
		{"FUf","RUr","BUb","LUl"},
		{"urUR","ubUB","ulUL","ufUF"},
		{"UFuf","URur","UBub","ULul"},
		{"urUR","ubUB","ulUL","ufUF"},
		{"UFuf","URur","UBub","ULul"},
		{"UruR","UbuB","UluL","UfuF"},
		{"uFUf","uRUr","uBUb","uLUl"}
	};

	int distance_edge_corner = corners[pairID] % 8 - edges[pairID] % 8;
	if (distance_edge_corner < 0)
		distance_edge_corner += 4;


	int typeOfCase = 0;
	int preoffset = 0;
	if (distance_edge_corner == 0) { //8+1
		if (edges[pairID] / 8 == 0) {
			typeOfCase = 8;
		}
		else {
			typeOfCase = 1;
		}
	}
	else if (distance_edge_corner == 1) { //6+3
		if (edges[pairID] / 8 == 0) {
			typeOfCase = 6;
		}
		else {
			typeOfCase = 3;
		}
	}
	else if (distance_edge_corner == 2) { //4+5
		if (edges[pairID] / 8 == 0) {
			typeOfCase = 4;
		}
		else {
			typeOfCase = 5;
		}
	}
	else if (distance_edge_corner == 3) { //2+7
		if (edges[pairID] / 8 == 0) {
			typeOfCase = 2;
		}
		else {
			typeOfCase = 7;
		}
	}
	preoffset += corners[pairID] % 8 - 4 - pairID;
	if (preoffset > 3)
		preoffset -= 4;
	if (preoffset < 0)
		preoffset += 4;
	string move = "";
	string premove = "";
	switch (preoffset) {
	case 0:
		premove = "";
		break;
	case 1:
		premove = "u";
		break;
	case 2:
		premove = "uu";
		break;
	case 3:
		premove = "U";
		break;
	}

	move = premove + firstPhase[typeOfCase - 1][pairID] + secondPhase[typeOfCase - 1][pairID];
	//move = premove;
	//cerr << "move = " << move << endl;
	//cerr << premove << "\t" << firstPhase[typeOfCase - 1][pairID] << "\t" << secondPhase[typeOfCase - 1][pairID] << endl;
	//cerr << "move = " << move << endl;
	cerr << "5-" << typeOfCase << "[" << pairID << "]\t";
	code_s = move;
	code_input_index = 0;
	}
	//6th case: Corner in bottom, edge in middle
	else if ((corners[0] % 8 < 4 && edges[0] % 8 < 4 && (corners[0] / 8 != 0 || edges[0] / 8 != 0)) || (corners[1] % 8 < 4 && edges[1] % 8 < 4 && (corners[1] / 8 != 0 || edges[1] / 8 != 0)) || (corners[2] % 8 < 4 && edges[2] % 8 < 4 && (corners[2] / 8 != 0 || edges[2] / 8 != 0)) || (corners[3] % 8 < 4 && edges[3] % 8 < 4 && (corners[3] / 8 != 0 || edges[3] / 8 != 0))) {
	int pairID;
	if (corners[0] % 8 < 4 && edges[0] % 8 < 4 && (corners[0] / 8 != 0 || edges[0] / 8 != 0))
		pairID = 0;
	else if (corners[1] % 8 < 4 && edges[1] % 8 < 4 && (corners[1] / 8 != 0 || edges[1] / 8 != 0))
		pairID = 1;
	else if (corners[2] % 8 < 4 && edges[2] % 8 < 4 && (corners[2] / 8 != 0 || edges[2] / 8 != 0))
		pairID = 2;
	else if (corners[3] % 8 < 4 && edges[3] % 8 < 4 && (corners[3] / 8 != 0 || edges[3] / 8 != 0))
		pairID = 3;

	string firstPhase[5][4] = {
		{"rURuFuuf","bUBuRuur","lULuBuub","fUFuLuul"},
		{"rURuruuR","bUBubuuB","lULuluuL","fUFufuuF"},
		{"rURUruR","bUBUbuB","lULUluL","fUFUfuF"},
		{"ruRUrUR","buBUbUB","luLUlUL","fuFUfUF"},
		{"rURuFUf","bUBuRUr","lULuBUb","fUFuLUl"}
	};
	string secondPhase[5][4] = {
		{"uFuuf","uRuur","uBuub","uLuul"},
		{"urUR","ubUB","ulUL","ufUF"},
		{"UruuR","UbuuB","UluuL","UfuuF"},
		{"uuFUf","uuRUr","uuBUb","uuLUl"},
		{"UFUf","URUr","UBUb","ULUl"}
	};
	//if (edges[pairID] / 8 == 0) //dobrze
	//	cerr << ": to ";
	//else
	//	cerr << ": inne ";
	int typeOfCase = 0;
	if (corners[pairID] / 8 == 0) { //git
		if (edges[pairID] / 8 == 0) {
			typeOfCase = -1; //ulozone juz
			cerr << "error_6_" << pairID << "_" << edges[pairID] << "_" << corners[pairID] << endl;
		}
		else
			typeOfCase = 1;
	}
	else if (corners[pairID] / 8 == 1) { //blue
		if (edges[pairID] / 8 == 0)
			typeOfCase = 3;
		else
			typeOfCase = 5;
	}
	else if (corners[pairID] / 8 == 2) { //red
		if (edges[pairID] / 8 == 0)
			typeOfCase = 2;
		else
			typeOfCase = 4;
	}
	string midmove = "";
	int postoffset = edges[pairID] % 8 - pairID;
	int secondPhaseMove = edges[pairID] % 8;
	switch (postoffset) {
	case 0:
		midmove = "";
		break;
	case 1:
	case -3:
		midmove = "u";
		secondPhaseMove--;
		break;
	case -1:
	case 3:
		secondPhaseMove++;
		midmove = "U";
		break;
	case 2:
	case -2:
		secondPhaseMove -= 2;
		midmove = "uu";
		break;
	default:
		cerr << "error_postoffset_midmove: " << postoffset << endl;
	}
	if (secondPhaseMove > 3)
		secondPhaseMove -= 4;
	else if (secondPhaseMove < 0)
		secondPhaseMove += 4;

	cerr << "6-" << typeOfCase << "[" << pairID << "]\t";
	string move;
	move = firstPhase[typeOfCase - 1][edges[pairID] % 8] + midmove + secondPhase[typeOfCase - 1][secondPhaseMove];
	code_s = move;
	code_input_index = 0;
	//cerr << move << endl;
	}


	//cerr << code_s << endl << endl;
	//code_input_index = -1;
	return code_input_index;
}

//saves rotated tab1 to tab2
void rotateMatrix(bool tab1[5][5], bool tab2[5][5]) 
{
	tab2[2][2] = tab1[2][2];
	int N = 5;
	for (int x = 0; x < N / 2; x++) {
		for (int y = x; y < N - x - 1; y++) {
			tab2[x][y] = tab1[y][N - 1 - x];
			tab2[y][N - 1 - x] = tab1[N - 1 - x][N - 1 - y];
			tab2[N - 1 - x][N - 1 - y] = tab1[N - 1 - y][x];
			tab2[N - 1 - y][x] = tab1[x][y];
		}
	}
}

bool currentOrientaion[4][5][5];
bool orientations[57][5][5];
string move[57];
int o_index = 0;
void o_add(
	bool a11, bool a12, bool a13, bool a14, bool a15,
	bool a21, bool a22, bool a23, bool a24, bool a25,
	bool a31, bool a32, bool a33, bool a34, bool a35,
	bool a41, bool a42, bool a43, bool a44, bool a45,
	bool a51, bool a52, bool a53, bool a54, bool a55,
	string move) {

	orientations[o_index][0][0] = a51;
	orientations[o_index][0][1] = a52;
	orientations[o_index][0][2] = a53;
	orientations[o_index][0][3] = a54;
	orientations[o_index][0][4] = a55;

	orientations[o_index][1][0] = a41;
	orientations[o_index][1][1] = a42;
	orientations[o_index][1][2] = a43;
	orientations[o_index][1][3] = a44;
	orientations[o_index][1][4] = a45;

	orientations[o_index][2][0] = a31;
	orientations[o_index][2][1] = a32;
	orientations[o_index][2][2] = a33;
	orientations[o_index][2][3] = a34;
	orientations[o_index][2][4] = a35;

	orientations[o_index][3][0] = a21;
	orientations[o_index][3][1] = a22;
	orientations[o_index][3][2] = a23;
	orientations[o_index][3][3] = a24;
	orientations[o_index][3][4] = a25;

	orientations[o_index][4][0] = a11;
	orientations[o_index][4][1] = a12;
	orientations[o_index][4][2] = a13;
	orientations[o_index][4][3] = a14;
	orientations[o_index][4][4] = a15;
	o_index++;
}

void showOrientation(bool Orientaion[5][5]) {
	for (int i = 4; i >= 0; i--) {
		for (int j = 0; j < 5; j++) {
			if ((i == 0 && j == 0) || (i == 0 && j == 4))
				cerr << " ";
			else if ((i == 4 && j == 0) || (i == 4 && j == 4))
				cerr << " ";
			else if (Orientaion[i][j] == true)
				cerr << "#";
			else
				cerr << ".";
			cerr << " ";
		}
		cerr << "\n";
	}
	cerr << endl;
}
void getTopOrientation(bool currentOrientaion[4][5][5]) {
	//currentOrientaion[1][0] = ;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			currentOrientaion[0][i][j] = false;
	//middle
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (sides[u][i][j] == 1)
				currentOrientaion[0][i + 1][j + 1] = true;
	//edges
	for (int i = 0; i < 3; i++)
		if (sides[f][2][i] == 1)
			currentOrientaion[0][0][i+1] = true;
	for (int i = 0; i < 3; i++)
		if (sides[r][2][i] == 1)
			currentOrientaion[0][i+1][4] = true;
	for (int i = 0; i < 3; i++)
		if (sides[b][2][i] == 1)
			currentOrientaion[0][4][3 - i] = true;
	for (int i = 0; i < 3; i++)
		if (sides[l][2][i] == 1)
			currentOrientaion[0][3 - i][0] = true;
	//save every rotation
	rotateMatrix(currentOrientaion[0], currentOrientaion[1]);
	rotateMatrix(currentOrientaion[1], currentOrientaion[2]);
	rotateMatrix(currentOrientaion[2], currentOrientaion[3]);
}
int a = 0;

string test(string in) {
	cerr << endl << in;
	for (int i = 0; i < in.length(); i++) {
		char t = in[i];
		if (t >= '0' && t <= '9');
		else if (t > 'Z')
			t += 'Z' - 'z';
		else
			t -= 'Z' - 'z';
		in.erase(i, 1);
		in.insert(i, string(1, t));
	}
	string out = "";
	for (int i = 0; i < in.length(); i++) {
		out.insert(0, string(1, in[i]));
	}
	cerr << " -> " << out << endl;
	return out;
}

int oll() {
	getTopOrientation(currentOrientaion);

	//showOrientation(currentOrientaion[0]);
		//showOrientation(currentOrientaion[1]);
		//showOrientation(currentOrientaion[2]);
		//showOrientation(currentOrientaion[3]);
		//cerr << "\n\n========================================\n\n";

	//here we go
	code_input_index = 0;
	//dot
	bool x = true;
	o_add(
		0, 0, 1, 0, 0,
		1, 0, 0, 0, 1,
		1, 0, 1, 0, 1,
		1, 0, 0, 0, 1,
		0, 0, 1, 0, 0,
		"ruBrbrrURfrF"
	);
	o_add(
		0, 0, 1, 0, 0,
		1, 0, 0, 0, 1,
		1, 0, 1, 0, 1,
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		"RfrFuuRfrffuuf"
	);
	o_add(
		0, 1, 1, 0, 0,
		0, 0, 0, 0, 1,
		1, 0, 1, 0, 1,
		1, 0, 0, 1, 0,
		0, 0, 1, 0, 0,
		"bulULBUfruRUF"
	);
	o_add(
		0, 0, 1, 0, 0,
		1, 0, 0, 1, 0,
		1, 0, 1, 0, 1,
		0, 0, 0, 0, 1,
		0, 1, 1, 0, 0,
		"RuuRfrFUFUfUr"
	);
	o_add(
		0, 0, 1, 1, 0,
		0, 1, 0, 0, 0,
		1, 0, 1, 0, 1,
		1, 0, 0, 1, 0,
		0, 0, 1, 0, 0,
		"ruRuRfrFuuRfrF"
	);
	o_add(
		0, 0, 1, 0, 0,
		0, 1, 0, 1, 0,
		1, 0, 1, 0, 1,
		0, 1, 0, 1, 0,
		0, 0, 1, 0, 0,
		"MuumuuMumuuMuum"
	);
	o_add(
		0, 0, 1, 0, 0,
		0, 1, 0, 1, 0,
		1, 0, 1, 0, 1,
		1, 0, 0, 0, 1,
		0, 0, 1, 0, 0,
		"muruRUMRfrF"
	);
	o_add(
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0,
		1, 0, 1, 0, 1,
		0, 1, 0, 1, 0,
		0, 0, 1, 0, 0,
		"fruRuFuuFlfL"
	);
	o_add(
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		"xxxxx"
	);
	//line


	code_s = test("fruRuFuuFlfL");
	o_add(
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		"xxxxx"
	);
	showOrientation(orientations[0]);




	return 1;
}

