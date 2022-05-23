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
		rotation.z = M_PI / 2. - time + (rot[2] - 1) * M_PI / 2.;
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
		rotation.x = M_PI / 2. + time + (rot[1] - 1) * M_PI / 2.;
		return;
	}
}