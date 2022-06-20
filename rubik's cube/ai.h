#pragma once
#include <string>
using namespace std;
enum sides_code { down = 0, up = 1, right = 2, left = 3, front = 4, back = 5, d = 0, u = 1, r = 2, l = 3, f = 4, b = 5 };
// (0): white, (1): yellow, (2): red,   (3): orange, (4): blue,  (5): green
// (0): down,  (1): up,     (2): right, (3): left,   (4): front, (5): back

extern int code_input_index;
extern string code_s;

void letsGoAiStart();
void letsGoAiLoop(int ai_go, int* order);
void letsGoAiEnd();