#include "Tile.h"
#include <iostream>
extern VAO *vao_x, *vao_y, *vao_z;

Tile::Tile(glm::vec3 position_in, glm::vec3 color_in, glm::vec3 rotation_in, short side_in) {
	position = position_in;
	color = color_in;
	rotation = rotation_in;
	rotation2 = glm::vec3(0.0f, 0.0f, 0.0f);
	side = side_in;

	rotate = 0;
}
/*
tiles[0].position.x = sqrt(2) * sin(multi + tiles[0].posOffset) + 1;
tiles[0].position.y = sqrt(2) * cos(multi + tiles[0].posOffset) + 1;
*/
void Tile::genRotationPosition(float multi, float offset) {
	switch(side) {
	case 0: //front
	case 1: //back
		position.x = offset * sin(multi + posOffset) + 1;
		position.y = offset * cos(multi + posOffset) + 1;
		rotation.z = -multi;
		break;
	case 2: // bottom
	case 3: // top
		position.x = offset * sin(multi + posOffset) + 1;
		position.z = offset * cos(multi + posOffset) - 1.5;
		rotation.z = -multi;
		break;
	case 4: // left
	case 5: // right
		position.y = offset * sin(multi + posOffset) + 1;
		position.z = offset * cos(multi + posOffset) - 1.5;
		rotation.z = -multi;
		break;
	case 7: //center
		rotation.z = -multi;
		break;
	}

}

void Tile::genRotationPositionPerpendicular(int toSide, float multi, float offset) {
	//std::cerr << multi << "\n";
	switch (toSide) {
	case 0: //front
		position.x = offset * sin(multi + posOffset) + 1;
		position.y = offset * cos(multi + posOffset) + 1;
		//if (side == 2 || side == 3)
			rotation.z = M_PI / 2. -multi;
		//else
		//	rotation.z = -multi;
		break;
	}

}


double genPosOffsetCrossZAxis(float x, float y) {
	double result;

	if (x < 0.5 && x > -0.5) //left
		result = 2;
	else if (x < 2.5 && x > 1.5) //right
		result = 0;
	else if (y < 2.5 && y > 1.5) //down
		result = 1;
	else //up
		result = 3;
	return result * M_PI / 2.;
}
double genPosOffsetDiagZAxis(float x, float y) {
	double result;

	if (x > 1 && y > 1) //rightup
		result = 2;
	else if (x > 1 && y < 1) //rightdown
		result = 0;
	else if (x < 1 && y > 1) //leftup
		result = 1;
	else //leftdown
		result = 3;
	return result * M_PI / 2. + M_PI / 4.;
}
double genPosOffsetCrossXYAxis(float x, float y) {
	double result;

	if (x < 0.5 && x > -0.5) //left
		result = 2;
	else if (x < 2.5 && x > 1.5) //right
		result = 0;
	else if (y > -3 && y < -2) //down
		result = 1;
	else //up
		result = 3;
	return result * M_PI / 2.;
}
double genPosOffsetDiagXYAxis(float x, float y) {
	double result;

	if (x > 1 && y < -1.5) //rightup
		result = 2;
	else if (x > 1 && y > -1.5) //rightdown
		result = 0;
	else if (x < 1 && y < -1.5) //leftup
		result = 1;
	else //leftdown
		result = 3;
	return result * M_PI / 2. + M_PI / 4.;
}

double genPosOffsetCrossPerpendicular(float x, float y) {
	double result;
	if (x < 0.25) //left
		result = 2;
	else if (x > 1.75) //right
		result = 0;
	else if (y < 1) //down
		result = 1;
	else //up
		result = 3;
	return result * M_PI / 2.;
}
void Tile::genPositionOffset(bool isOnCross) {
	if (side % 2)
		if (isOnCross == true)
			isOnCross = false;
		else
			isOnCross = true;

	switch (side) {
	case 0: //front
	case 1: //back
		if (isOnCross == true)
			posOffset = genPosOffsetCrossZAxis(position.x, position.y);
		else
			posOffset = genPosOffsetDiagZAxis(position.x, position.y);
		break;
	case 2: //bottom
	case 3: //top
		if (isOnCross == true)
			posOffset = genPosOffsetCrossXYAxis(position.x, position.z);
		else
			posOffset = genPosOffsetDiagXYAxis(position.x, position.z);
		break;
	case 4: //left
	case 5: //right
		if (isOnCross == true)
			posOffset = genPosOffsetCrossXYAxis(position.y, position.z);
		else
			posOffset = genPosOffsetDiagXYAxis(position.y, position.z);
		break;
	}
}

void Tile::genPositionOffsetPerpendicular(bool isOnCross) {

	std::cerr << "x= " << position.x << "\ty= " << position.y << "\tz=" << position.z;
	//if (side % 2)
	//	if (isOnCross == true)
	//		isOnCross = false;
	//	else
	//		isOnCross = true;

	//switch (side) {
	//case 2: //bottom
		if (isOnCross == true)
			//posOffset = 1 * M_PI / 2.;
			posOffset = genPosOffsetCrossPerpendicular(position.x, position.y);
		else
			posOffset = 0 * M_PI / 2. + M_PI / 4.;
			//genPosOffsetDiagXYAxis(position.x, position.z);
	//	break;
	//}
	std::cerr << "\t" << posOffset / M_PI * 2. <<std::endl;
}

void Tile::updateSide(int toSide) {
	std::cerr << "\t\t" << side << "\n";
	switch (toSide) {
	case 0:
		if (side == 2) side = 5;
		else if (side == 5) side = 3;
		else if (side == 3) side = 4;
		else if (side == 4) side = 2;
		
		if (side == 4 || side == 5)
			vao = vao_y;
		else
			vao = vao_x;
		rotation = glm::vec3(0, 0, 0);

		break;

	}




}