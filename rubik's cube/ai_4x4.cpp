#include "Block.h"
#include "ai_4x4.h"
#include <iostream>
#include <string>
using std::cerr; using std::cout; using std::endl; using std::string;

extern int N;
extern Block* GLOBALblocks;
namespace four {
	int*** sides;
	int* ai_toRotate;
	
	void showSides() {
		string names[6] = { "down\t","up\t","right\t","left\t","front\t","back\t" };
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
		for (int i = N - 1; i >= 0; i--) {
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
	void generateSidesTables() {
		sides = new int** [6];
		for (int i = 0; i < 6; i++) {
			sides[i] = new int* [N];
			for (int j = 0; j < N; j++) {
				sides[i][j] = new int[N];
			}
		}
	}
	void updateSidesFromOrder(int* order) {
		for (int i = 0; i < N; i++) { //front + back
			for (int j = 0; j < N; j++) {
				sides[0][N - 1 - i][N - 1 - j] = GLOBALblocks[order[(i * N + j) % N + ((i * N + j) / N) * N * N]].color[5];//down
				sides[1][i][N - 1 - j] = GLOBALblocks[order[(i * N + j) % N + ((i * N + j) / N) * N * N + N * (N - 1)]].color[4];//up

				sides[2][i][j] = GLOBALblocks[order[((i * N + j) % N) * N * N + ((i * N + j) / N) * N]].color[3];//right
				sides[3][i][N - 1 - j] = GLOBALblocks[order[((i * N + j) % N) * N * N + ((i * N + j) / N) * N + (N - 1)]].color[1];//left

				sides[4][i][N - 1 - j] = GLOBALblocks[order[i * N + j]].color[0];//front
				sides[5][i][j] = GLOBALblocks[order[i * N + j + N * N * (N - 1)]].color[2];//back
			}
		}
	}
	void delete3dArray(int*** tab) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < N; j++) {
				delete[] tab[i][j];
			}
			delete[] tab[i];
		}
		delete[] tab;
	}
	void clear_4x4() {
		delete[] ai_toRotate;
		delete3dArray(sides);
	}
	void init_4x4() {
		ai_toRotate = new int[pow(N, 2)];
		generateSidesTables();
	}

	void white_side() {

		showSides();
		


	}
}