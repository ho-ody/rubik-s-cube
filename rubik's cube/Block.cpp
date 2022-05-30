#include "Block.h"
#include <iostream>

Block::Block() {
	rotation = glm::vec3(0, 0, 0);
	for (int i = 0; i < 6; i++)
		color[i] = glm::vec3(0.1, 0.1, 0.1);
}

extern int N; extern float v; //skalowanie
void Block::rotate(int n, int axis, float time, int direction) {
	float offset, posOffset, blockOffset;

	if (N % 2 == 1) { //odd cube size
		if (axis == 2) { //red
			if (offset_[2] == 0) { //cross
				offset = radius_[2] * 2 * v;
				posOffset = 0.;
				blockOffset = M_PI / 2. * blockOffsetFix;
			}
			else { //not cross
				//offset = sqrt(pow((N - 1) * v, 2) + pow(abs(n) * (N - 1) * v, 2));
				offset = sqrt(pow(offset_[2], 2) + pow(radius_[2], 2)) * 2 * v;
				posOffset = atan(offset_[2] / radius_[2]);
				blockOffset = M_PI / 2. * blockOffsetFix;
				//offset = posOffset = blockOffset = 0.;
				if (offsetSideFix[2] == true) {
					posOffset *= -1;
					blockOffset += M_PI / 2.;
					//for (int l = 0; l < 6; l++)
					//	color[l] = glm::vec3(0.7, 0, 0.7);
				}
			}
			if (blockOffsetFix != -1) { //center fix
				if (direction) {
					position.x = offset * sin(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
				}
				else {
					position.x = offset * sin(time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(time + posOffset + blockOffset) + (N - 1) * v;
				}
			}
			if (direction)
				rotation.z = 3 * M_PI / 2. + time + (rot[2] - prevRot[2] - 1) * M_PI / 2.;
			else
				rotation.z = 3 * M_PI / 2. - time + (rot[2] - prevRot[2] - 1) * M_PI / 2.;
			return;
		}
		if (axis == 0) { //yellow
			if (offset_[0] == 0) { //cross
				offset = radius_[0] * 2 * v;
				posOffset = 0.;
				blockOffset = M_PI / 2. * blockOffsetFix;
			}
			else { //not cross
				offset = sqrt(pow(offset_[0], 2) + pow(radius_[0], 2)) * 2 * v;
				posOffset = atan(offset_[0] / radius_[0]);
				blockOffset = M_PI / 2. * blockOffsetFix;
				if (offsetSideFix[0] == true) {
					posOffset *= -1;
					blockOffset += M_PI / 2.;
				}
			}
			if (blockOffsetFix != -1) { //center fix
				if (direction) {
					position.z = offset * sin(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
				}
				else {
					position.z = offset * sin(time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(time + posOffset + blockOffset) + (N - 1) * v;
				}
			}
			if (direction)
				rotation.x = M_PI / 2. - time + (rot[0] - prevRot[0] - 1) * M_PI / 2.;
			else
				rotation.x = M_PI / 2. + time + (rot[0] - prevRot[0] - 1) * M_PI / 2.;
			return;
		}
		if (axis == 1) { //green
			if (offset_[1] == 0) { //cross
				offset = radius_[1] * 2 * v;
				posOffset = 0.;
				blockOffset = M_PI / 2. * blockOffsetFix;
			}
			else { //not cross
				offset = sqrt(pow(offset_[1], 2) + pow(radius_[1], 2)) * 2 * v;
				posOffset = atan(offset_[1] / radius_[1]);
				blockOffset = M_PI / 2. * blockOffsetFix;
				if (offsetSideFix[1] == true) {
					posOffset *= -1;
					blockOffset += M_PI / 2.;
				}
			}
			if (blockOffsetFix != -1) { //center fix
				if (direction) {
					position.x = offset * sin(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
					position.z = offset * cos(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
				}
				else {
					position.x = offset * sin(time + posOffset + blockOffset) + (N - 1) * v;
					position.z = offset * cos(time + posOffset + blockOffset) + (N - 1) * v;
				}
			}
			if (direction)
				rotation.y = M_PI / 2. - time + (rot[1] - prevRot[1] - 1) * M_PI / 2.;
			else
				rotation.y = M_PI / 2. + time + (rot[1] - prevRot[1] - 1) * M_PI / 2.;
			return;
		}
	}
	else { //even cube size
		if (axis == 2) { //red
			offset = sqrt(pow(offset_[2]+.5, 2) + pow(radius_[2]+.5, 2)) * 2 * v;
			posOffset = atan((offset_[2]+.5) / (radius_[2]+.5));
			blockOffset = M_PI / 2. * blockOffsetFix;
			if (offsetSideFix[2] == true ) {
				posOffset *= -1;
				blockOffset += M_PI / 2.;
				}
			if (blockOffsetFix != -1) { //center fix
				if (direction) {
					position.x = offset * sin(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
				}
				else {
					position.x = offset * sin(time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(time + posOffset + blockOffset) + (N - 1) * v;
				}
			}
			if (direction)
				rotation.z = 3 * M_PI / 2. + time + (rot[2] - prevRot[2] - 1) * M_PI / 2.;
			else
				rotation.z = 3 * M_PI / 2. - time + (rot[2] - prevRot[2] - 1) * M_PI / 2.;
			return;
		}
		if (axis == 0) { //yellow
			offset = sqrt(pow(offset_[0] + .5, 2) + pow(radius_[0] + .5, 2)) * 2 * v;
			posOffset = atan((offset_[0] + .5) / (radius_[0] + .5));
			blockOffset = M_PI / 2. * blockOffsetFix;
			if (offsetSideFix[0] == true) {
				posOffset *= -1;
				blockOffset += M_PI / 2.;
			}
			if (blockOffsetFix != -1) { //center fix
				if (direction) {
					position.z = offset * sin(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
				}
				else {
					position.z = offset * sin(time + posOffset + blockOffset) + (N - 1) * v;
					position.y = offset * cos(time + posOffset + blockOffset) + (N - 1) * v;
				}
			}
			if (direction)
				rotation.x = M_PI / 2. - time + (rot[0] - prevRot[0] - 1) * M_PI / 2.;
			else
				rotation.x = M_PI / 2. + time + (rot[0] - prevRot[0] - 1) * M_PI / 2.;
			return;
		}
		if (axis == 1) { //green
			offset = sqrt(pow(offset_[1] + .5, 2) + pow(radius_[1] + .5, 2)) * 2 * v;
			posOffset = atan((offset_[1] + .5) / (radius_[1] + .5));
			blockOffset = M_PI / 2. * blockOffsetFix;
			if (offsetSideFix[1] == true) {
				posOffset *= -1;
				blockOffset += M_PI / 2.;
			}
			if (blockOffsetFix != -1) { //center fix
				if (direction) {
					position.x = offset * sin(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
					position.z = offset * cos(M_PI / 2. - time + posOffset + blockOffset) + (N - 1) * v;
				}
				else {
					position.x = offset * sin(time + posOffset + blockOffset) + (N - 1) * v;
					position.z = offset * cos(time + posOffset + blockOffset) + (N - 1) * v;
				}
			}
			if (direction)
				rotation.y = M_PI / 2. - time + (rot[1] - prevRot[1] - 1) * M_PI / 2.;
			else
				rotation.y = M_PI / 2. + time + (rot[1] - prevRot[1] - 1) * M_PI / 2.;
			return;
		}
	}
}

void Block::update(int direction) {
	glm::vec3 t;
	for (int i = 0; i < abs( rot[0] - prevRot[0]); i++) {
		if (direction) {
			t = color[0];
			color[0] = color[5];
			color[5] = color[2];
			color[2] = color[4];
			color[4] = t;
		}
		else {
			t = color[0];
			color[0] = color[4];
			color[4] = color[2];
			color[2] = color[5];
			color[5] = t;
		}	
	}
	for (int i = 0; i <abs( rot[1] - prevRot[1]); i++) {
		if (direction) {
			t = color[0];
			color[0] = color[1];
			color[1] = color[2];
			color[2] = color[3];
			color[3] = t;
		}
		else {
			t = color[0];
			color[0] = color[3];
			color[3] = color[2];
			color[2] = color[1];
			color[1] = t;
		}
	}
	for (int i = 0; i <abs( rot[2] - prevRot[2]); i++) {
		if (direction) {
			t = color[1];
			color[1] = color[4];
			color[4] = color[3];
			color[3] = color[5];
			color[5] = t;
		}
		else {
			t = color[1];
			color[1] = color[5];
			color[5] = color[3];
			color[3] = color[4];
			color[4] = t;
		}
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