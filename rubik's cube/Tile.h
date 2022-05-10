#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>

class Tile
{
public:
	glm::vec3 position; //relative to center
	glm::vec3 color;
	glm::vec3 rotation;
	float posOffset;
	bool rotate;
	short side;

	void genRotationPosition(float, float);
	void genPositionOffset(bool isOnCross);

	Tile(glm::vec3 = glm::vec3(0.0, 0.0, 0.0), glm::vec3 = glm::vec3(0.3,0.3,0.3), glm::vec3 = glm::vec3(0.0, 0.0, 0.0), short = 7);
};
