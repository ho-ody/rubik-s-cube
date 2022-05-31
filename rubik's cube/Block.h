#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include "VAO.h"

class Block
{
public:
	glm::vec3 position; //relative to center
	// (-1): none, (0): white, (1): yellow, (2):red, (3): orange, (4): blue, (5): green
	int color[6];
	glm::vec3 rotation;
	int rot[3] = { 0,0,0 };
	int prevRot[3] = { 0,0,0 };

	bool roll = false;

	int blockOffsetFix;

	float offset_[3] = { 0., 0., 0. };
	float radius_[3] = { 0., 0., 0. };
	bool offsetSideFix[3] = { false, false, false };

	Block();
	void rotate(int n, int axis, float time, int direction);
	void update(int direction); //updates block - reset rotations, swap colors
};
