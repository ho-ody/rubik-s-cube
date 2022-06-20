#include "ai_3x3.h"
#include "ai.h"
#include "ai_rotate_symulation.h"
#include "source.h"
#include <iostream>
using namespace std;

bool solved = false;

void letsGoAiStart() {
	init_3x3();
}
bool once = true;
double stat_number = 0;
double stat_n = 0;
void letsGoAiLoop(int ai_go, int* order) {
	if (ai_go && code_input_index == -1) {
		if (once) {
			once = false;
			updateSidesFromOrder(order);
		}
		if (solved == false)
			if (cross() == -1)
				if (f2l() == -1)
				{
					oll();
					pll();
					solved = true;

					//cerr << endl << ai_wholecode << endl;	
					string ai_wholecode = simplify_final_code();
					cerr << endl << ai_wholecode << endl;

					stat_number += calc_length(ai_wholecode);
					stat_n++;
					//cerr << " avg= " << stat_number / stat_n << endl;

					//excel += to_string(stat_number / stat_n);
					//excel += "\n";

					code_s = ai_wholecode;
					ai_wholecode = "";
					code_input_index = 0;
					//generateScramble(50);
					//showSides();
				}
		//if (excel_go)
		//	copyToClipboard(excel);
	}
}
void letsGoAiEnd() {
	clear_3x3();
}