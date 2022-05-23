#include "Block.h"
#include <iostream>

Block::Block() {
	rotation = glm::vec3(0, 0, 0);
	for (int i = 0; i < 6; i++)
		color[i] = glm::vec3(0.1, 0.1, 0.1);
}

void Block::rotate(int n, int toSide, float time, int side) {

	float offset, posOffset, blockOffset;
	if (toSide == 0) { //red
		if (n == 0) { //cross
			offset = 1.0;
			posOffset = 0.;
			blockOffset = M_PI / 2. * blockOffsetFix;
		}
		else { //not cross
			offset = sqrt(1. + abs(n) * abs(n));
			posOffset = atan(n);
			blockOffset = M_PI / 2. * blockOffsetFix;
		}
		//float tx = position.x, ty = position.y;
		if (blockOffsetFix != -1) { //center fix
			position.x = offset * sin(time + posOffset + blockOffset) + 1;
			position.y = offset * cos(time + posOffset + blockOffset) + 1;
		}
		//if (side == 2 || side == 3)
		//rotation.x = M_PI / 2. * rot[1];
		rotation.z = M_PI / 2. - time + (rot[2] - prevRot[2] - 1) * M_PI / 2.;
		return;
	}
	if (toSide == 2) { //yellow
		if (n == 0) { //cross
			offset = 1.0;
			posOffset = 0.;
			blockOffset = M_PI / 2. * blockOffsetFix;
		}
		else { //not cross
			offset = sqrt(1. + abs(n) * abs(n));
			posOffset = atan(n);
			blockOffset = M_PI / 2. * blockOffsetFix;
		}
		//float tx = position.x, ty = position.y;
		if (blockOffsetFix != -1) { //center fix
			position.z = offset * sin(time + posOffset + blockOffset) + 1;
			position.y = offset * cos(time + posOffset + blockOffset) + 1;
		}
		//if (side == 2 || side == 3)
		rotation.x = M_PI / 2. + time + (rot[0] - prevRot[0] - 1) * M_PI / 2.;
		//rotation.z = M_PI / 2. * rot[2];
		return;
	}
}

bool eq(float a, float b) { //equal approximately
	if (a + 0.001 > b && a - 0.001 < b)
		return true;
	return false;
}


void Block::update() {
	/*
	int x_rot = round(rotation.x / M_PI * 2.);
	int y_rot = round(rotation.y / M_PI * 2.);
	int z_rot = round(rotation.z / M_PI * 2.);
	std::cerr << x_rot << "," << y_rot << "," << z_rot << "\n";
	*/
	glm::vec3 t;
	for (int i = 0; i < abs( rot[0] - prevRot[0]); i++) {
		t = color[0];
		color[0] = color[4];
		color[4] = color[2];
		color[2] = color[5];
		color[5] = t;
	}
	for (int i = 0; i <abs( rot[1] - prevRot[1]); i++) {
		t = color[1];
		color[1] = color[5];
		color[5] = color[3];
		color[3] = color[4];
		color[4] = t;
	}
	for (int i = 0; i <abs( rot[2] - prevRot[2]); i++) {
		t = color[1];
		color[1] = color[5];
		color[5] = color[3];
		color[3] = color[4];
		color[4] = t;
	}
	rotation = glm::vec3(0., 0., 0.);
	for (int i = 0; i < 3; i++)
		prevRot[i] = rot[i];
	/*
	blocks[i].color[0] = glm::vec3(0.9, 0.1, 0.1); //red
	blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
	blocks[i].color[2] = glm::vec3(0.9, 0.5, 0.1); //orange
	blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
	blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
	blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
	*/
}