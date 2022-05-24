#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include "VAO.h"

class Block
{
public:
	glm::vec3 position; //relative to center
	glm::vec3 color[6];
	glm::vec3 rotation;
	int rot[3] = { 0,0,0 };
	int prevRot[3] = { 0,0,0 };

	bool roll = false;

	int blockOffsetFix;

	float offset_ = 0.;
	float radius_ = 0.;

	Block();
	void rotate(int n, int axis, float time, int direction);
	void update(int direction); //updates block - reset rotations, swap colors
};
