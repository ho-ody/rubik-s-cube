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
	//glm::vec3 rotation2;
	//float posOffset;
	//int rotate;
	//short side;
	//VAO* vao;
	//int placeOfBlock;

	//void genRotationPosition(float, float);
	//void genRotationPositionPerpendicular(int toSide, float, float);
	//void genPositionOffset(bool isOnCross);
	//void genPositionOffsetPerpendicular(int toSide, int placeOfBlock);
	//void updateSide(int toSide);

	//Block(glm::vec3 = glm::vec3(0.0, 0.0, 0.0), glm::vec3 = glm::vec3(0.3, 0.3, 0.3), glm::vec3 = glm::vec3(0.0, 0.0, 0.0), short = 7);
	Block();
};
