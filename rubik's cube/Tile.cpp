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
extern double xxx, yyy;

void genOnCross(int toSide, float multi, float offset, float posOffset, glm::vec3& p, glm::vec3& r) {
	offset = 1.5; //!!!
	switch (toSide) {
	case 0: //front
		p.x = offset * sin(multi + posOffset) + 1;
		p.y = offset * cos(multi + posOffset) + 1;
		//if (side == 2 || side == 3)
		r.z = M_PI / 2. - multi;
		//else
		//	rotation.z = -multi;
		break;
	}
}

void genOnDiag(int toSide, float multi, float offset, float posOffset, int placeOfBlock, glm::vec3& p, glm::vec3& r) {
	offset = sqrt(1 * 1 + 1.5 * 1.5); //!!!

	float blockCorrect;
	if (placeOfBlock == 2)
		blockCorrect = atan(1.5 / 1.);
	else if (placeOfBlock == 0)
		blockCorrect = atan(1. / 1.5);

	switch (toSide) {
	case 0: //front
		p.x = offset * sin(multi + posOffset + blockCorrect) + 1;
		p.y = offset * cos(multi + posOffset + blockCorrect) + 1;
		//if (side == 2 || side == 3)
		r.z = M_PI / 2. - multi;
		//else
		//	rotation.z = -multi;
		break;
	}
}


void Tile::genRotationPositionPerpendicular(int toSide, float multi, float offset) {
	/*
	offset = sqrt(2) * 1.5;
	offset = sqrt(1 * 1 + 1.5 * 1.5);

	double blockCorrect = atan(1./1.5);
	//posOffset = 1 * M_PI / 2.;
	//posOffset = M_PI / 4.;
	switch (toSide) {
	case 0: //front
		position.x = offset * sin(multi + posOffset + blockCorrect) + 1;
		position.y = offset * cos(multi + posOffset + blockCorrect) + 1;
		//if (side == 2 || side == 3)
		rotation.z = M_PI / 2. -multi;
		//else
		//	rotation.z = -multi;
		break;
	}
	*/
	/*
	offset = sqrt(2) * 1.5;
	offset = sqrt(1 * 1 + 1.5 * 1.5);

	//double blockCorrect = atan(1. / 1.5);
	double blockCorrect = 0.;
	if (placeOfBlock == 1)
		blockCorrect = atan(1.5 / 1.);
	else if (placeOfBlock == 2)
		blockCorrect = atan(1. / 1.5);
	offset = 1.5;

	//posOffset = 1 * M_PI / 2.;
	//posOffset = M_PI / 4.;
	switch (toSide) {
	case 0: //front
		position.x = offset * sin(multi + posOffset + blockCorrect) + 1;
		position.y = offset * cos(multi + posOffset + blockCorrect) + 1;
		//if (side == 2 || side == 3)
		rotation.z = M_PI / 2. - multi;
		//else
		//	rotation.z = -multi;
		break;
	}
	*/
	if (placeOfBlock == 1)
		genOnCross(toSide, multi, offset, posOffset, position, rotation);
	else
		genOnDiag(toSide, multi, offset, posOffset, placeOfBlock, position, rotation);
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
double genPosOffsetDiagPerpendicular1(float x, float y) {
	double result;
	if (x < -0.25) //left
		result = 1;
	else if (x > 2.25) //right
		result = 3;
	else if (y < 1) //down
		result = 0; 
	else //up
		result = 2;
	//std::cerr << result << "\n";
	return result * M_PI / 2.;
}
double genPosOffsetDiagPerpendicular2(float x, float y) {
	double result;
	if (x < -0.25) //left
		result = 2;
	else if (x > 2.25) //right
		result = 0;
	else if (y < 1) //down
		result = 1;
	else //up
		result = 3;
	//std::cerr << result << "\n";
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

void Tile::genPositionOffsetPerpendicular(int placeOfBlock_in) {
	std::cerr << placeOfBlock << " -> " << placeOfBlock_in << std::endl;
	placeOfBlock = placeOfBlock_in;
	//std::cerr << "x= " << position.x << "\ty= " << position.y << "\tz=" << position.z << " -> ";
	//if (side % 2)
	//	if (isOnCross == true)
	//		isOnCross = false;
	//	else
	//		isOnCross = true;

	//switch (side) {
	//case 2: //bottom
	if (placeOfBlock == 1) //0->cross
		posOffset = genPosOffsetCrossPerpendicular(position.x, position.y);
	else if (placeOfBlock == 2) //1->diagn one
		posOffset = genPosOffsetDiagPerpendicular1(position.x, position.y);
	else //2->diagn one
		posOffset = genPosOffsetDiagPerpendicular2(position.x, position.y);
			//genPosOffsetDiagXYAxis(position.x, position.z);
	//	break;
	//}
	//std::cerr << "\t" << posOffset / M_PI * 2. <<std::endl;
}

void Tile::updateSide(int toSide) {
	
	std::cerr << "("<< position.x << "," << position.y << ")\t" << side << " -> ";
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
	std::cerr << side << "\n";



}