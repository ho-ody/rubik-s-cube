#include <iostream>
#include "Block.h"
using namespace std;

extern int N;
int*** sides;
extern int* order;
extern Block* GLOBALblocks;

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
			sides[0][i][j] = GLOBALblocks[order[(i * N + j) % N + ((i * N + j) / N) * N * N]].color[5];//up
			sides[1][i][j] = GLOBALblocks[order[(i * N + j) % N + ((i * N + j) / N) * N * N + N * (N - 1)]].color[4];//down

			sides[2][i][j] = GLOBALblocks[order[((i * N + j) % N) * N + ((i * N + j) / N) * N * N]].color[3];//right
			sides[3][i][j] = GLOBALblocks[order[((i * N + j) % N) * N + ((i * N + j) / N) * N * N + (N - 1)]].color[1];//left

			sides[4][i][j] = GLOBALblocks[order[i * N + j]].color[0];//front
			sides[5][i][j] = GLOBALblocks[order[i * N + j + N * N * (N - 1)]].color[2];//back
		}
	}
}

void showSides() {
	string names[6] = { "down","up","right","left","front","back" };
	for (int i = 0; i < 6; i++) {
		cerr << names[i] << "\t";
	}
	cerr << "\n";
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < 6; k++) {
			for (int j = 0; j < N; j++) {
				cerr << sides[k][i][j] << " ";
			}
			cerr << "\t";
		}
		cerr << "\n";
	}
	cerr << "\n";
}

void letsGoAiLoop() {
	updateSidesFromOrder();
	/*
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cerr << GLOBALblocks[order[i * 3 + j]].color[0] << "\t"; //front
		}
		cerr << endl;
	}
	cerr << endl;
	*/
	//cerr << sides << endl;
	showSides();
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


void cross() {



}